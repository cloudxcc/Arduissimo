//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_gpio.h"
#include "driver_spi.h"

///////////////////////////////////////////////////
//	spi_master_init
///////////////////////////////////////////////////
void spi_master_init(unsigned id, char sclk_pin, char mosi_pin, char miso_pin, char mode, unsigned delay) {
   // save the configuration
   spi_master_mode[id] = mode;
   // save the day
   spi_master_delay[id] = delay;
   // link master final
   int dummy = CA_CT;  // workaround
   spi_master_hash[id] = spi_master_final(id, dummy);
   ///////////////////////////////////
   // SCLK
   ///////////////////////////////////
   // get GPIO_N basic register
   unsigned* gpio_n = &GPIO_0_OUT_CLR;
   gpio_n += 0x40 * (sclk_pin >> 3);
   // get bit number
   unsigned bit_mask = 0x1 << (sclk_pin & 0x7);
   spi_master_sclk_bit[id] = bit_mask;
   // save clr and set addresses
   if (mode & 0x2) { // CPOL = 1
     spi_master_sclk_lead_add[id] = gpio_n;
     gpio_n += 0x1;
     spi_master_sclk_trail_add[id] = gpio_n;
     // set bit
     *gpio_n = bit_mask;
   } else {        // CPOL = 0
      spi_master_sclk_trail_add[id] = gpio_n;
      // set bit
      *gpio_n = bit_mask;
      gpio_n += 0x1;
      spi_master_sclk_lead_add[id] = gpio_n;
   }
   // set output
   set_dir(sclk_pin, 1);
   ///////////////////////////////////
   // MOSI
   ///////////////////////////////////
   // get GPIO_N basic register
   gpio_n = &GPIO_0_OUT_CLR;
   gpio_n += 0x40 * (mosi_pin >> 3);
   // get bit number
   bit_mask = 0x1 << (mosi_pin & 0x7);
   spi_master_mosi_bit[id] = bit_mask;
   // save clr and set addresses
   spi_master_mosi_clr_add[id] = gpio_n;
   gpio_n += 0x1;
   spi_master_mosi_set_add[id] = gpio_n;
   // set bit
   *gpio_n = bit_mask;
   // set output
   set_dir(mosi_pin, 1);
   ///////////////////////////////////
   // MISO
   ///////////////////////////////////
   // get GPIO_N basic register
   gpio_n = &GPIO_0_IN;
   gpio_n += 0x40 * (miso_pin >> 3);
   spi_master_miso_bank[id] = gpio_n;
   // get bit number
   bit_mask = 0x1 << (miso_pin & 0x7);
   spi_master_miso_bit[id] = bit_mask;
   // set input
   set_dir(miso_pin, 0);
}

///////////////////////////////////////////////////
//	spi_master_ss_init
///////////////////////////////////////////////////
void spi_master_ss_init(unsigned tag, char ss_pin) {
   spi_master_slave_pin[tag] = ss_pin;
   // set level 1
   set_pin(ss_pin, 1);
   // set output
   set_dir(ss_pin, 1);
}

///////////////////////////////////////////////////
//	spi_master_write
///////////////////////////////////////////////////
void spi_master_write(unsigned tag, char ss_id, unsigned data) {
   // prepare write_data frame
   spi_master_write_data[tag] = data;        // data

   // code CPHA [6] and SS-pin [31:7] into state
   int state = ((spi_master_mode[tag] & 0x1) << 6) +    // save CPHA
               (spi_master_slave_pin[ss_id] << 7);   // save SS-pin

   spi_master_state[tag] = 0 - state;

   spi_master_read_data[tag] = 0;   // why not for clear as well

   unsigned trigger = CA_CT + 600;  // 120 cycle when 4 threads running,
                                    // 480 for maximal number of threads

   spi_master_trigger[tag] = trigger;
   spi_master_event(tag, trigger);
}

///////////////////////////////////////////////////
//	spi_master_event
// state is counting like -72d, 71d, 70d, ... 0
///////////////////////////////////////////////////
void spi_master_event(unsigned tag, int start_time) {
   if (start_time >= 0) {
      CA_COM = 0xc0000000 +                             // execute threa
               (tag << 14) +                            // save tag
               (((unsigned)&&spi_master_event_label >> 1) & 0x3fff); // set pc
      CA_ET = start_time;
   } else {
      unsigned* gpio_n;
      unsigned bit_mask, ss_pin, data, trigger;
      int state;
      spi_master_event_label:
         // get state
         state = spi_master_state[tag];
         ///////////////////////////////
         // stop condition
         ///////////////////////////////
         if ((state & 0x1f) == 17)
         {
            // set SS pin
            ss_pin = state >> 7;
            gpio_n = &GPIO_0_OUT_SET;
            gpio_n += 0x40 * (ss_pin >> 3);
            *gpio_n = 0x1 << (ss_pin & 0x7);
            // set MOSI to 1 per default
            gpio_n = spi_master_mosi_set_add[tag];
            bit_mask = spi_master_mosi_bit[tag];
            *gpio_n = bit_mask;
            TC_START = spi_master_hash[tag];
         } else
         {
            ///////////////////////////////
            // start condition, CPHA = 0
            ///////////////////////////////
            if (state < 0)
            {
               // state value als contains SS pin and CPHA
               state = 0 - state;
               // clear SS pin
               ss_pin = state >> 7;
               gpio_n = &GPIO_0_OUT_CLR;
               gpio_n += 0x40 * (ss_pin >> 3);
               *gpio_n = 0x1 << (ss_pin & 0x7);
               // MOSI depending from CPHA handling
               if ((state & 0x40) == 0) { // CPHA = 0
                  data = spi_master_write_data[tag];
                  // MOSI is set to 1 per default
                  if ((data & 0x1) == 0) {
                     gpio_n = spi_master_mosi_clr_add[tag];
                     bit_mask = spi_master_mosi_bit[tag];
                     *gpio_n = bit_mask;
                  }
               }
            } else
            ///////////////////////////////
            // leading edge
            ///////////////////////////////
            if ((state & 0x1) == 0x1)
            {
               if (state & 0x40) { // CPHA = 1
                  data = spi_master_write_data[tag];
               if ((data >> ((state >> 1) & 0xf)) & 0x1)
                  gpio_n = spi_master_mosi_set_add[tag];
               else
                  gpio_n = spi_master_mosi_clr_add[tag];
                  bit_mask = spi_master_mosi_bit[tag];
                  *gpio_n = bit_mask;
               } else {
                  // read MISO
                  gpio_n = spi_master_miso_bank[tag];
                  unsigned read = *gpio_n;  // maybe a little bit late in the game
                  data = spi_master_read_data[tag];
                  data >>= 1;
                  if (read & spi_master_miso_bit[tag])
                     data += 0x1 << 7;
                  spi_master_read_data[tag] = data;;
               }
               gpio_n = spi_master_sclk_lead_add[tag];
               bit_mask = spi_master_sclk_bit[tag];
               *gpio_n = bit_mask;
            } else
            ///////////////////////////////
            // trailing edge
            ///////////////////////////////
            {
               // MOSI depending from CPHA handling
               if ((state & 0x40) == 0) { // CPHA = 0
                  data = spi_master_write_data[tag];
                  if ((data >> ((state >> 1) & 0xf)) & 0x1)
                     gpio_n = spi_master_mosi_set_add[tag];
                  else
                     gpio_n = spi_master_mosi_clr_add[tag];
                  bit_mask = spi_master_mosi_bit[tag];
                  *gpio_n = bit_mask;
               } else {
                  // read MISO
                  gpio_n = spi_master_miso_bank[tag];
                  unsigned read = *gpio_n;  // maybe a little bit late in the game
                  data = spi_master_read_data[tag];
                  data >>= 1;
                  if (read & spi_master_miso_bit[tag])
                  data += 0x1 << 7;
                  spi_master_read_data[tag] = data;;
               }
               gpio_n = spi_master_sclk_trail_add[tag];
               bit_mask = spi_master_sclk_bit[tag];
               *gpio_n = bit_mask;
            }
            // update trigger
            trigger = spi_master_trigger[tag] + spi_master_delay[tag];
            // re-arm the calendar
            CA_COM = 0xc0000000 +                             // execute thread
                     (tag << 14) +                            // save id
                     (((unsigned)&&spi_master_event_label >> 1) & 0x3fff); // set pc
            CA_ET = trigger;
            // save trigger
            spi_master_trigger[tag] = trigger;
            // update and save state
            spi_master_state[tag] = state + 1;
         }
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	spi_slave_init
///////////////////////////////////////////////////
void spi_slave_init(unsigned id, char ss_pin, char sclk_pin, char mosi_pin, char miso_pin, char mode) {
   // save the configuration
   spi_slave_mode[id] = mode;
   // link slave final
   int dummy = CA_CT;  // workaround
   spi_slave_hash[id] = spi_slave_final(id, dummy);
   // link gpio event
   gpio_event(0x0, dummy);
   ///////////////////////////////////
   // SS
   ///////////////////////////////////
   // link event execution
   unsigned event_hash = spi_slave_event(id, dummy);
   gpio_event_hash[ss_pin] = event_hash;
   // get GPIO_N_IN register
   unsigned* gpio_n = &GPIO_0_IN;
   gpio_n += 0x40 * (ss_pin >> 3);
   spi_slave_ss_bank[id] = gpio_n;
   // get bit number
   unsigned bit_mask = 0x1 << (ss_pin & 0x7);
   // save bit number
   spi_slave_ss_bit[id] = ss_pin & 0x7;
   // set input, why not
   set_dir(ss_pin, 0);
   ///////////////////////////////////
   // SCLK
   ///////////////////////////////////
   // link event execution
   gpio_event_hash[sclk_pin] = event_hash;
   // get GPIO_N_IN register
   gpio_n = &GPIO_0_CAP;
   gpio_n += 0x40 * (sclk_pin >> 3);
   spi_slave_sclk_bank[id] = gpio_n;
   // get bit number
   bit_mask = 0x1 << (sclk_pin & 0x7);
   // save bit number
   spi_slave_sclk_bit[id] = sclk_pin & 0x7;
   // set input, why not
   set_dir(sclk_pin, 0);
   ///////////////////////////////////
   // MISO
   ///////////////////////////////////
   // get GPIO_N basic register
   gpio_n = &GPIO_0_OUT_CLR;
   gpio_n += 0x40 * (miso_pin >> 3);
   // get bit number
   bit_mask = 0x1 << (miso_pin & 0x7);
   spi_slave_miso_bit[id] = bit_mask;
   // save clr and set addresses
   spi_slave_miso_clr_add[id] = gpio_n;
   gpio_n += 0x1;
   spi_slave_miso_set_add[id] = gpio_n;
   // set bit
   *gpio_n = bit_mask;
   // set output
   set_dir(miso_pin, 1);
   ///////////////////////////////////
   // MOSI
   ///////////////////////////////////
   // get GPIO_N basic register
   gpio_n = &GPIO_0_IN;
   gpio_n += 0x40 * (mosi_pin >> 3);
   spi_slave_mosi_bank[id] = gpio_n;
   // get bit number
   bit_mask = 0x1 << (mosi_pin & 0x7);
   spi_slave_mosi_bit[id] = bit_mask;
   // set input
   set_dir(mosi_pin, 0);
}

///////////////////////////////////////////////////
//	spi_slave_enable
///////////////////////////////////////////////////
unsigned spi_slave_enable(unsigned id, unsigned data) {
   // reset read data
   spi_slave_read_data[id] = 0;
   // reset slave state
   // save write data in state variable
   // code CPHA [6] CPOL [5] into state
   unsigned state = 0x1000000 +                        // set 1 at the end
                    (data << 16) +                     // save data
                    ((spi_slave_mode[id] & 0x3) << 5); // save CPHA and CPOL
   spi_slave_state[id] = state;
   // get GPIO_N_IN register
   unsigned* gpio_n_in = spi_slave_ss_bank[id];
   // enable falling edge GPIO event at ss_pin
   gpio_n_in += 0x4;
   *gpio_n_in = 0x1 << spi_slave_ss_bit[id];
}

///////////////////////////////////////////////////
//	spi_slave_event
///////////////////////////////////////////////////
unsigned spi_slave_event(unsigned tag, int start_time) {
   if (start_time >= 0) {
     return 0xc0000000 +                          // execute thread
            (tag << 14) +                         // save id
            (((unsigned)&&spi_slave_label >> 1) & 0x3fff); // set pc
   } else
   {
      unsigned* gpio_n;
      unsigned state, data, read;
      spi_slave_label:
         // read state
         state = spi_slave_state[tag];
         // check for end of frame
         if ((state & 0x1f) == 0x11) {
            gpio_n = spi_slave_miso_set_add[tag];
            unsigned bit_mask = spi_slave_miso_bit[tag];
            *gpio_n = bit_mask;
            TC_START = spi_slave_hash[tag];
         } else {
            // leading edge
            if (state & 0x1) {
               // save MOSI
               if ((state & 0x20) == 0) {
                  gpio_n = spi_slave_mosi_bank[tag];
                  read = *gpio_n;
                  data = spi_slave_read_data[tag];
                  data >>= 1;
                  if (read & spi_slave_mosi_bit[tag])
                     data += 0x1 << 7;
                  spi_slave_read_data[tag] = data;
               }
               // enable next edge GPIO event at sclk_pin
               gpio_n = spi_slave_sclk_bank[tag];
               char sclk_bit = spi_slave_sclk_bit[tag];
               if (state & 0x40)
                  gpio_n -= 0x3;
               else
                  gpio_n -= 0x4;
               *gpio_n = 0x1 << sclk_bit;
               // output MISO
               if (state & 0x20) {
                  if ((state >> (((state >> 1) & 0xf) + 16)) & 0x1)
                     gpio_n = spi_slave_miso_set_add[tag];
                  else
                     gpio_n = spi_slave_miso_clr_add[tag];
                  unsigned bit_mask = spi_slave_miso_bit[tag];
                  *gpio_n = bit_mask;
               }
            // trailing edge
            } else {
               // end of frame
               if ((state & 0x1f) == 0x10) {
                  gpio_n = spi_slave_ss_bank[tag];
                  // enable rising edge GPIO event at ss_pin
                  gpio_n += 0x5;
                  *gpio_n = 0x1 << spi_slave_ss_bit[tag];
               }
               // save MOSI
               if (state & 0x20) {
                  gpio_n = spi_slave_mosi_bank[tag];
                  read = *gpio_n;
                  data = spi_slave_read_data[tag];
                  data >>= 1;
                  if (read & spi_slave_mosi_bit[tag])
                     data += 0x1 << 7;
                  spi_slave_read_data[tag] = data;
               }
               // enable next edge GPIO event at sclk_pin
               if ((state & 0x1f) != 0x10) {
                  gpio_n = spi_slave_sclk_bank[tag];
                  char sclk_bit = spi_slave_sclk_bit[tag];
                  if (state & 0x40)
                     gpio_n -= 0x4;
                  else
                     gpio_n -= 0x3;
                 *gpio_n = 0x1 << sclk_bit;
               }
               // output MISO
               if ((state & 0x20) == 0) {
                  if ((state >> (((state >> 1) & 0xf) + 16)) & 0x1)
                     gpio_n = spi_slave_miso_set_add[tag];
                  else
                     gpio_n = spi_slave_miso_clr_add[tag];
                  unsigned bit_mask = spi_slave_miso_bit[tag];
                  *gpio_n = bit_mask;
               }
            }
            spi_slave_state[tag] = state + 1;
         }
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}
