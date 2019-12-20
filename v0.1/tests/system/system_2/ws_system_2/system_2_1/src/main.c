//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

int main(void)
{
   // synchronization with testbench
   unsigned i;
   for (int i = 0; i < 10; i++)
   {
      GPIO_0_OUT_SET = 0x1;
      GPIO_0_OUT_CLR = 0x1;
   }
   // write to core 2
   for (i = 0; i < 8; i++)
   {
      MP_OUT_2 = 7 - i;
   }
   // write to core 3
   for (i = 0; i < 8; i++)
   {
      MP_OUT_3 = 7 - i;
   }  

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");

}
