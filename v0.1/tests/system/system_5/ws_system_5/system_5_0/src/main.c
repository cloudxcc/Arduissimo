//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

void thread_0 (int start_time) {
   if (start_time >= 0) {
      CA_COM = 0x80000000 + (((unsigned)&&t0_label >> 1) & 0x3ff);
      CA_ET = start_time;
   } else
   {
      t0_label:
         GPIO_0_OUT_SET = 0x1;
         TC_KILL = 0x0;
       asm volatile ("addi x0, x0, 0");
   }
}

void thread_1 (int start_time) {
   if (start_time >= 0) {
      CA_COM = 0x80000000 + (((unsigned)&&t1_label >> 1) & 0x3ff);
      CA_ET = start_time;
   } else
   {
      t1_label:
         GPIO_0_OUT_SET = 0x2;
         TC_KILL = 0x0;
       asm volatile ("addi x0, x0, 0");
   }
}

void start(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000+ (((unsigned)&&start_label >> 1) & 0x3ff);
   } else
   {
      unsigned data;
      start_label:
         data = GPIO_2_IN;
         // forcing 2 consecutive gpio reads
         asm volatile ("lw   a6,800(a5)"); // GPIO_3_IN
         GPIO_2_OUT_SET = data;
         asm volatile ("sw   a6,788(a5)"); // GPIO_3_OUT_SET
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

void start_and_kill(int start_time) {
   if (start_time >= 0) {
      TC_SAK = 0xc0000000+ (((unsigned)&&sak_label >> 1) & 0x3ff);
   } else
   {
      sak_label:
         GPIO_0_OUT_SET = 0x4;
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

int main(void)
{
   GPIO_0_DIR_SET = 0xff;
   GPIO_1_DIR_SET = 0xff;
   GPIO_2_DIR_SET = 0xff;

   int CT = CA_CT;

   // insert CA, ca_match at rd_gpio
   thread_0(CT + 0x1ab);
   // insert CA, ca_match at start
   thread_1(CT + 0x1e7);

   GPIO_1_OUT_SET = GPIO_1_IN;
   start(CT);
   start_and_kill(CT);

   GPIO_0_OUT_SET = 0x8; // this must not be executed

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
