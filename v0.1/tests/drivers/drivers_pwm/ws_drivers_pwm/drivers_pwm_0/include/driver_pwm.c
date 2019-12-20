//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_pwm.h"

///////////////////////////////////////////////////
//	pwm_init
///////////////////////////////////////////////////
void pwm_init(unsigned id, char pwm_pin, unsigned high, unsigned low) {
   // save the day
   pwm_low[id] = low;
   // save high time
   pwm_high[id] = high;
   ///////////////////////////////////
   // pin set/clr command
   ///////////////////////////////////
   pwm_clr_com[id] = //0x00000000 +
                     ((pwm_pin >> 3) << 8) +
                     (0x1 << (pwm_pin & 0x7));
   pwm_set_com[id] = 0x20000000 +
                     ((pwm_pin >> 3) << 8) +
                     (0x1 << (pwm_pin & 0x7));
   // set output
   set_dir(pwm_pin, 1);
   ///////////////////////////////////
   // basics
   ///////////////////////////////////
   pwm_state[id] = 0;

   unsigned trigger = CA_CT + 600;  // 120 cycle when 4 threads running,
                                    // 480 for maximal number of threads

   pwm_trigger[id] = trigger;
   pwm_event(id, trigger);
}

///////////////////////////////////////////////////
//	pwm_event
///////////////////////////////////////////////////
void pwm_set(unsigned tag, unsigned high, unsigned low) {
   // save high time
   pwm_high[tag] = high;
   // save low time
   pwm_low[tag] = low;
}

///////////////////////////////////////////////////
//	pwm_event
///////////////////////////////////////////////////
void pwm_event(unsigned tag, int start_time) {
   if (start_time >= 0) {
     CA_COM = 0xc0000000 +                             // execute threa
              (tag << 14) +                            // save tag
              (((unsigned)&&pwm_event_label >> 1) & 0x3fff); // set pc
     CA_ET = start_time;
   } else {
      unsigned* gpio_n_out;
      unsigned state, bit_mask, trigger;
      pwm_event_label:
         state = pwm_state[tag];
         if (state)
         {
            // save state
            pwm_state[tag] = 0;
            // update trigger
            trigger = pwm_trigger[tag] + pwm_low[tag];
            // next trigger set pin
            CA_COM = pwm_set_com[tag];
         } else
         {
            // save state
            pwm_state[tag] = 1;
            // update trigger
            trigger = pwm_trigger[tag] + pwm_high[tag];
            // next trigger clr pin
            CA_COM = pwm_clr_com[tag];
         }
         // save trigger
         pwm_trigger[tag] = trigger;
         CA_ET = trigger;
         // re-arm the calendar
         CA_COM = 0xc0000000 +                             // execute thread
                  (tag << 14) +                            // save id
                  (((unsigned)&&pwm_event_label >> 1) & 0x3fff); // set pc
         CA_ET = trigger;
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}
