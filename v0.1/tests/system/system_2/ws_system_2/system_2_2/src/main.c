//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

int main(void)
{
   // read from core 1
   unsigned i;
   for (i = 0; i < 8; i++)
   {
      unsigned read = MP_IN_1;
      GPIO_1_OUT_SET = 0x1 << read;
   }

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");

}
