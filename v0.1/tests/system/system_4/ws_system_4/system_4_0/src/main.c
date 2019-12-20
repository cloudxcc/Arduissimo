//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

char c_data [8];
unsigned short s_data [8];
unsigned u_data [1024];

int main(void)
{
   // write pattern in c_data memory
   for (int i = 0; i < 8; i += 1)
   {
      c_data[i] = (char)i;
   }
   // read pattern from c_data memory
   for (int i = 0; i < 8; i += 1)
   {
      char tmp = c_data[i];
      GPIO_0_OUT_SET = 0x1 << tmp;
   }

   // write pattern in s_data memory
   for (int i = 0; i < 8; i += 1)
   {
      s_data[i] = i + (i << 8);
   }
   // read pattern from s_data memory
   for (int i = 0; i < 8; i += 1)
   {
      short tmp = s_data[i];
      GPIO_1_OUT_SET = 0x1 << (tmp & 0xff);
      GPIO_2_OUT_SET = 0x1 << ((tmp >> 8) & 0xff);
   }

   // write pattern in u_data memory
   for (int i = 0; i < 8; i += 1)
   {
      u_data[i] = i +
                  (i <<  8) +
                  (i << 16) +
                  (i << 24);
   }
   // read pattern from s_data memory
   for (int i = 0; i < 8; i += 1)
   {
      unsigned tmp = u_data[i];
      GPIO_3_OUT_SET = 0x1 << (tmp & 0xff);
      GPIO_4_OUT_SET = 0x1 << ((tmp >>  8) & 0xff);
      GPIO_5_OUT_SET = 0x1 << ((tmp >> 16) & 0xff);
      GPIO_6_OUT_SET = 0x1 << ((tmp >> 24) & 0xff);
   }

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");

}
