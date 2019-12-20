//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

void gpio(unsigned tag, int start_time) {
   if (start_time >= 0) {
      GPIO_EVENT_ADD = (((unsigned)&&gpio_label >> 1) & 0x3fff);
   } else
   {
      gpio_label:
         switch (tag >> 3) {
            case 0:  GPIO_0_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 1:  GPIO_1_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 2:  GPIO_2_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 3:  GPIO_3_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 4:  GPIO_4_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 5:  GPIO_5_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 6:  GPIO_6_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 7:  GPIO_7_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 8:  GPIO_8_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 9:  GPIO_9_OUT_SET  = 0x1 << (tag & 0x7); break;
            case 10: GPIO_10_OUT_SET = 0x1 << (tag & 0x7); break;
            case 11: GPIO_11_OUT_SET = 0x1 << (tag & 0x7); break;
            default : ;
         }
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

int main(void)
{
   // program gpio event add
   int CT = CA_CT;
   gpio(0x0, CT);

   // program gpio level
   GPIO_0_LVL1 = 0xff;
   GPIO_1_LVL1 = 0xff;
   GPIO_2_LVL1 = 0xff;
   GPIO_3_LVL1 = 0xff;
   GPIO_4_LVL1 = 0xff;
   GPIO_5_LVL1 = 0xff;
   GPIO_6_LVL1 = 0xff;
   GPIO_7_LVL1 = 0xff;
   GPIO_8_LVL1 = 0xff;
   //GPIO_9_LVL1 = 0xff;
   //GPIO_10_LVL1 = 0xff;
   //GPIO_11_LVL1 = 0xff;

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");

}
