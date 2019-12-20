//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_gpio.h"
#include "driver_spi.h"

///////////////////////////////////////////////////
//	spi_master_final
///////////////////////////////////////////////////
unsigned spi_master_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                                          // execute thread, set a0
             (tag << 14) +                                         // save tag
             (((unsigned)&&spi_master_final_label >> 1) & 0x3fff); // set pc
   } else {
      spi_master_final_label:
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

///////////////////////////////////////////////////
//	spi_slave_final
///////////////////////////////////////////////////
unsigned spi_slave_final(unsigned tag, int start_time) {
   if (start_time >= 0) {
      return 0xc0000000 +                                         // execute thread, set a0
             (tag << 14) +                                        // save tag
             (((unsigned)&&spi_slave_final_label >> 1) & 0x3fff); // set pc
   } else {
      spi_slave_final_label:
         GPIO_0_OUT_SET = spi_slave_read_data[tag];
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}
