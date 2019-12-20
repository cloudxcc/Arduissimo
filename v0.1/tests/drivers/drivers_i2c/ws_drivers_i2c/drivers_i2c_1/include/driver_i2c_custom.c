//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_gpio.h"
#include "driver_i2c.h"

///////////////////////////////////////////////////
//	i2c_master_final
///////////////////////////////////////////////////
unsigned i2c_master_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                                          // execute thread, set a0
             (tag << 14) +                                         // save tag
             (((unsigned)&&i2c_master_final_label >> 1) & 0x3fff); // set pc
   } else {
      unsigned data;
      i2c_master_final_label:
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	i2c_slave_final
///////////////////////////////////////////////////
unsigned i2c_slave_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                                         // execute thread, set a0
             (tag << 14) +                                        // save tag
             (((unsigned)&&i2c_slave_final_label >> 1) & 0x3fff); // set pc
   } else {
      i2c_slave_final_label:
         if (program_state == 0) {
            GPIO_0_OUT_SET = i2c_slave_read_data[tag];
            i2c_slave_enable(0, 0x81);
            program_state += 1;
         }
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}
