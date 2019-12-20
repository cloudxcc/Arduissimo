//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

void mp(unsigned tag, int start_time) {
   if (start_time >= 0) {
      MP_COM_1 = 0x00004000 + (((unsigned)&&mp_label >> 1) & 0x3fff);
   } else
   {
      int i;
      mp_label:
         GPIO_2_OUT_SET = 0x1 << tag;

         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

int main(void)
{
   // write to core 2
   int CT = CA_CT;
   mp(0x0, CT);

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");

}
