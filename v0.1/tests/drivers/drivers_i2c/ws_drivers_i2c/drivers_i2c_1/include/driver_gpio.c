//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_gpio.h"

void set_pin (unsigned pin, unsigned level) {
   unsigned* gpio_n_out = &GPIO_0_OUT_CLR;
   gpio_n_out += 0x40 * (pin >> 3);
   if (level)
      gpio_n_out += 0x1;
   *gpio_n_out = 0x1 << (pin & 0x7);
}

void set_dir (unsigned pin, unsigned dir) {
   unsigned* gpio_n_dir = &GPIO_0_DIR_CLR;
   gpio_n_dir += 0x40 * (pin >> 3);
   gpio_n_dir += dir;
   *gpio_n_dir = 0x1 << (pin & 0x7);
}

void gpio_event(unsigned tag, int start_time) {
   if (start_time >= 0) {
     GPIO_EVENT_ADD = (((unsigned)&&gpio_event_label >> 1) & 0x3fff);
   } else
   {
     gpio_event_label:
       TC_SAK = gpio_event_hash[tag];
       //TC_START = gpio_event_hash[tag];
       //TC_KILL = 0x0;
       asm volatile ("addi x0, x0, 0");
   }
}


