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
      return 0xc0000000 +                                // execute thread, set a0
             (tag << 14) +                               // save tag
             (((unsigned)&&i2c_master_final_label >> 1) & 0x3fff); // set pc
   } else {
      unsigned data;
      i2c_master_final_label:
         if (tag == 0) {
            data = (i2c_master_write_data[0] >> 1) & 0xff;
            if (data < 15 * 0x10) {
               ca_delay(10000);
               i2c_master_write(0, 0x40, data + 0x10);
            }
         }
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	i2c_slave_final
///////////////////////////////////////////////////
unsigned i2c_slave_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                               // execute thread, set a0
             (tag << 14) +                              // save tag
             (((unsigned)&&i2c_slave_final_label >> 1) & 0x3fff); // set pc
   } else {
      unsigned data;
      i2c_slave_final_label:
         if (tag < 7) {
            data = i2c_slave_read_data[tag] & 0xff;
            ca_delay(((data & 0xf) + ((data & 0xff) >> 4)) * 10);
            i2c_master_write(tag + 1, 0x40 + tag + 1, data + 1);
         }
         i2c_slave_enable(tag, 0);
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}