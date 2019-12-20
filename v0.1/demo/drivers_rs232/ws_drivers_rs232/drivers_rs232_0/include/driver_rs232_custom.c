//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_gpio.h"
#include "driver_rs232.h"

unsigned gl_tx_cnt;

///////////////////////////////////////////////////
//	rs232_tx_final
///////////////////////////////////////////////////
unsigned rs232_tx_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                              // execute thread, set a0
             (tag << 14) +                              // save tag
             (((unsigned)&&rs232_tx_final_label >> 1) & 0x3fff); // set pc
   } else {
      rs232_tx_final_label:
         if (tag == 0) {
            if (gl_tx_cnt < 15 * 16) {
               //ca_delay(10000);
               gl_tx_cnt += 16;
               rs232_tx(tag, gl_tx_cnt);
            }
         }
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	rs232_rx_final
///////////////////////////////////////////////////
unsigned rs232_rx_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                              // execute thread, set a0
             (tag << 14) +                              // save tag
             (((unsigned)&&rs232_rx_final_label >> 1) & 0x3fff); // set pc
   } else {
      rs232_rx_final_label:
         GPIO_3_OUT_CLR = 0xff;
         GPIO_3_OUT_SET = ~rs232_rx_data[tag];
         ca_delay(((rs232_rx_data[tag] & 0xf) + ((rs232_rx_data[tag] & 0xff) >> 4)) * 10);
         rs232_tx(tag + 1, (rs232_rx_data[tag] & 0xff) + 1);
         rs232_rx_rearm(tag);
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}
