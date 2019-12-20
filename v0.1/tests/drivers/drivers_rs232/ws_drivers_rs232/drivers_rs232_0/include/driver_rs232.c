//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_gpio.h"
#include "driver_rs232.h"

///////////////////////////////////////////////////
//	rs232_tx_init
///////////////////////////////////////////////////
void rs232_tx_init(unsigned id, char tx_pin, char parity, char stop_bits, unsigned delay) {
   // save the configuration
   rs232_tx_conf[id] = ((parity & 0x3) << 1) + (stop_bits & 0x1);
   // save the day
   rs232_tx_delay[id] = delay;
   // get GPIO_N basic register
   unsigned* gpio_n_out = &GPIO_0_OUT_CLR;
   gpio_n_out += 0x40 * (tx_pin >> 3);
   // save clr and set addresses
   rs232_tx_clr_add[id] = gpio_n_out;
   gpio_n_out += 0x1;
   rs232_tx_set_add[id] = gpio_n_out;
   // get bit number
   unsigned bit_mask = 0x1 << (tx_pin & 0x7);
   rs232_tx_bit[id] = bit_mask;
   // set bit to 1 (before setting output)
   *gpio_n_out = bit_mask;
   // set output
   set_dir(tx_pin, 1);
   // link TX final
   int dummy = CA_CT;  // workaround
   rs232_tx_hash[id] = rs232_tx_final(id, dummy);
}

///////////////////////////////////////////////////
//	rs232_tx
///////////////////////////////////////////////////
void rs232_tx(unsigned tag, unsigned data) {
   if (rs232_tx_conf[tag] & 0x6) {
      // add parity
      unsigned parity = 0;
      for (int n = 0; n < 8; n++)
      {
         parity += (data >> n) & 0x1;
      }
      if (rs232_tx_conf[tag] & 0x4)
         parity += 0x1;
      data += (parity & 0x1) << 8;
      // add stop bit(s)
      if (rs232_tx_conf[tag] & 0x1)
         data += 0xe00;
      else
         data += 0x600;
      // save frame
      rs232_tx_data[tag] = data << 1;
   } else
   {
      // add stop bit(s)
      if (rs232_tx_conf[tag] & 0x1)
         data += 0x700;
      else
         data += 0x300;
      // save frame
       rs232_tx_data[tag] = data << 1;
   }
   unsigned CT = CA_CT + 600;  // 120 cycle when 4 threads running,
                               // 480 for maximal number of threads
   rs232_tx_trigger[tag] = CT;
   rs232_tx_event(tag, CT);
}

///////////////////////////////////////////////////
//	rs232_tx_event
///////////////////////////////////////////////////
void rs232_tx_event(unsigned tag, int start_time) {
   if (start_time >= 0) {
      CA_COM = 0xc0000000 +                                      // execute threa
               (tag << 14) +                                     // save tag
               (((unsigned)&&rs232_tx_bit_label >> 1) & 0x3fff); // set pc
      CA_ET = start_time;
   } else {
      unsigned* gpio_n_out;
      unsigned bit_mask, data, trigger;
      rs232_tx_bit_label:
         // get data
         data = rs232_tx_data[tag];
         // continue ?
         if ((data & 0xffe) != 0)
         {
            // set or clear output pin
            if (data & 0x1)
               gpio_n_out = rs232_tx_set_add[tag];
            else
               gpio_n_out = rs232_tx_clr_add[tag];
            bit_mask = rs232_tx_bit[tag];
            *gpio_n_out = bit_mask;
            // update data
            rs232_tx_data[tag] = data >> 0x1;
            // next event
            trigger = rs232_tx_trigger[tag] + rs232_tx_delay[tag];
            // re-arm the calendar
            CA_COM = 0xc0000000 +                                      // execute thread
                     (tag << 14) +                                     // save id
                     (((unsigned)&&rs232_tx_bit_label >> 1) & 0x3fff); // set pc
            CA_ET = trigger;
            rs232_tx_trigger[tag] = trigger;
         } else
            TC_START = rs232_tx_hash[tag];
      TC_KILL = 0x0;
      asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	rs232_rx_init
///////////////////////////////////////////////////
void rs232_rx_init(unsigned id, char rx_pin, char parity, unsigned delay) {
   // save the configuration
   rs232_rx_conf[id] = parity & 0x3;
   // save the day
   rs232_rx_delay[id] = delay;
   // clear data
   rs232_rx_data[id] = 0;
   // link event execution
   int dummy = CA_CT;  // workaround
   gpio_event_hash[rx_pin] = rs232_rx(id, dummy);
   // link gpio event
   gpio_event(0x0, dummy);
   // link rx final
   rs232_rx_hash[id] = rs232_rx_final(id, dummy);
   // get GPIO_N_IN register
   unsigned* gpio_n_in = &GPIO_0_IN;
   gpio_n_in += 0x40 * (rx_pin >> 3);
   rs232_rx_bank[id] = gpio_n_in;
   // get bit number
   unsigned bit_mask = 0x1 << (rx_pin & 0x7);
   // enable falling edge GPIO event at rx_pin
   gpio_n_in += 0x4;
   *gpio_n_in = bit_mask;
   // save bit number
   rs232_rx_bit[id] = rx_pin & 0x7;
   // set input, why not
   //set_dir(rx_pin, 0);
}

///////////////////////////////////////////////////
//	rs232_rx
///////////////////////////////////////////////////
unsigned rs232_rx(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                                  // execute thread
             (tag << 14) +                                 // save id
             (((unsigned)&&rs232_rx_label >> 1) & 0x3fff); // set pc
   } else
   {
      unsigned* gpio_n_in;
      unsigned read, data, trigger;
      rs232_rx_label:
      // read input bank close to thread start
      gpio_n_in = rs232_rx_bank[tag];
      read = *gpio_n_in;
      // get data, indicating protocol position
      data = rs232_rx_data[tag];
      if ((data & 0x1) != 0) {   // Parity or stop bit
         data >>= 1;
         data += (read << (8 - rs232_rx_bit[tag])) & 0x100;
         rs232_rx_data[tag] = data;
         TC_START = rs232_rx_hash[tag];
      } else
      {
         if (data == 0) {      // start bit
            // save new trigger time
            trigger = CA_CT + ((rs232_rx_delay[tag] * 3) >> 1) - 70;
            // save protocol state in data
            if (rs232_rx_conf[tag] == 0)
               rs232_rx_data[tag] = 0x100;
            else
               rs232_rx_data[tag] = 0x200;
         } else {
            // save new trigger time
            trigger = rs232_rx_trigger[tag] + rs232_rx_delay[tag];
            // save protocol state and receive data
            data >>= 1;
            //data += 0x100;
            if (rs232_rx_conf[tag] == 0)
               data += (read << (8 - rs232_rx_bit[tag])) & 0x100;
            else
               data += (read << (9 - rs232_rx_bit[tag])) & 0x200;
            rs232_rx_data[tag] = data;
       }
          // re-arm the calendar
          CA_COM = 0xc0000000 +                                 // execute thread
                   (tag << 14) +                                 // save id
                   (((unsigned)&&rs232_rx_label >> 1) & 0x3fff); // set pc
          CA_ET = trigger;
          // save new trigger time
          rs232_rx_trigger[tag] = trigger;
      }
      TC_KILL = 0x0;
      asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	rs232_rx_rearm
///////////////////////////////////////////////////
void rs232_rx_rearm(unsigned id) {
   // clear data
   rs232_rx_data[id] = 0;
   // get GPIO_N_IN register
   unsigned* gpio_n_in = rs232_rx_bank[id]; //&GPIO_0_IN;
   // get bit number
   unsigned bit_mask = 0x1 << rs232_rx_bit[id]; //(rx_pin & 0x7);
   // enable falling edge GPIO event at rx_pin
   gpio_n_in += 0x4;
   *gpio_n_in = bit_mask;
}

