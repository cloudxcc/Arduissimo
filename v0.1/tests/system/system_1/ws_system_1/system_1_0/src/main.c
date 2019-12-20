//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

void gpio0(unsigned tag, int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (tag << 14) + (((unsigned)&&gpio0_label >> 1) & 0x3ff);
   } else
   {
      int i;
      gpio0_label:
         GPIO_0_OUT_SET = 0x1 << tag;
         GPIO_0_OUT_CLR = 0x1 << tag;
         i = 0;
         do {
            GPIO_0_OUT_SET = 0x1 << tag;
            GPIO_0_OUT_CLR = 0x1 << tag;
            i++;
         } while (i < 80);
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

void gpio1(unsigned tag, int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (tag << 14) + (((unsigned)&&gpio1_label >> 1) & 0x3ff);
   } else
   {
      int i;
      gpio1_label:
         GPIO_1_OUT_SET = 0x1 << tag;
         GPIO_1_OUT_CLR = 0x1 << tag;
         i = 0;
         do {
            GPIO_1_OUT_SET = 0x1 << tag;
            GPIO_1_OUT_CLR = 0x1 << tag;
            i++;
         } while (i < 80);
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

void gpio2(unsigned tag, int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (tag << 14) + (((unsigned)&&gpio2_label >> 1) & 0x3ff);
   } else
   {
      int i;
      gpio2_label:
         GPIO_2_OUT_SET = 0x1 << tag;
         GPIO_2_OUT_CLR = 0x1 << tag;
         i = 0;
         do {
            GPIO_2_OUT_SET = 0x1 << tag;
            GPIO_2_OUT_CLR = 0x1 << tag;
            i++;
         } while (i < 80);
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

void thread_0 (int start_time) {
   if (start_time >= 0) {
      CA_COM = 0x80000000 + (((unsigned)&&t0_label >> 1) & 0x3ff);
      CA_ET = start_time;
   } else
   {
      t0_label:
         GPIO_2_OUT_CLR = 0x1;
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
         GPIO_2_OUT_SET = 0x1;
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

int main(void)
{
   GPIO_0_DIR_SET = 0xff;
   GPIO_1_DIR_SET = 0xff;

   GPIO_0_OUT_SET = 0x1;
   GPIO_0_OUT_CLR = 0x1;

   int CT = CA_CT;

   // 3 threads
   gpio0(0x1, CT);
   gpio0(0x2, CT);
   // insert CA at empty
   thread_1(CT + 0x402);
   // insert CA at reinsert
   thread_0(CT + 0x597);
   for (int i = 0; i < 100; i++)
   {
      GPIO_0_OUT_SET = 0x1;
      GPIO_0_OUT_CLR = 0x1;
   }

   // 4 threads
   CT = CA_CT;
   gpio0(0x1, CT);
   gpio0(0x2, CT);
   gpio0(0x3, CT);
   // insert CA at empty
   thread_1(CT + 0x402);
   // insert CA at reinsert
   thread_0(CT + 0x597);
   for (int i = 0; i < 100; i++)
   {
      GPIO_0_OUT_SET = 0x1;
      GPIO_0_OUT_CLR = 0x1;
   }

   // 6 threads
   CT = CA_CT;
   gpio0(0x1, CT);
   gpio0(0x2, CT);
   gpio0(0x3, CT);
   gpio0(0x4, CT);
   gpio0(0x5, CT);
   // insert CA at empty
   thread_1(CT + 0x600);
   // insert CA at reinsert
   thread_0(CT + 0x800);
   for (int i = 0; i < 100; i++)
   {
      GPIO_0_OUT_SET = 0x1;
      GPIO_0_OUT_CLR = 0x1;
   }

   // 16 threads
   CT = CA_CT;
   gpio0(0x1, CT);
   gpio0(0x2, CT);
   gpio0(0x3, CT);
   gpio0(0x4, CT);
   gpio0(0x5, CT);
   gpio0(0x6, CT);
   gpio0(0x7, CT);
   gpio1(0x0, CT);
   gpio1(0x1, CT);
   gpio1(0x2, CT);
   gpio1(0x3, CT);
   gpio1(0x4, CT);
   gpio1(0x5, CT);
   gpio1(0x6, CT);
   gpio1(0x7, CT);
   gpio2(0x0, CT);

   for (int i = 0; i < 100; i++)
   {
      GPIO_0_OUT_SET = 0x1;
      GPIO_0_OUT_CLR = 0x1;
   }

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");

}
