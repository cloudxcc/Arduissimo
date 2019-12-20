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
//	rs232_tx_final
///////////////////////////////////////////////////
unsigned rs232_tx_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                                        // execute thread, set a0
             (tag << 14) +                                       // save tag
             (((unsigned)&&rs232_tx_final_label >> 1) & 0x3fff); // set pc
   } else {
      rs232_tx_final_label:
         GPIO_2_OUT_SET = 0xff;
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	rs232_rx_final
///////////////////////////////////////////////////
unsigned rs232_rx_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                                        // execute thread, set a0
             (tag << 14) +                                       // save tag
             (((unsigned)&&rs232_rx_final_label >> 1) & 0x3fff); // set pc
   } else {
      rs232_rx_final_label:
         GPIO_0_OUT_SET = rs232_rx_data[tag];
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}


