//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "adpcm.c"

int main (void)
{
   unsigned* tb = (unsigned*)0x1001200c;

   while (1)
   {
      int i;
      int main_result;

      main_result = 0;
      adpcm_main ();
      for (i = 0; i < IN_END / 2; i++)
      {
         if (compressed[i] != test_compressed[i])
         {
            main_result += 1;
         }
      }
      for (i = 0; i < IN_END; i++)
      {
         if (result[i] != test_result[i])
         {
            main_result += 1;
         }
      }
      if (main_result != 0x0)
      {
         *tb = 0x80000;
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         *tb = 0x80000;
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         break;
      }

      *tb = 0x400000;
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     *tb = 0x400000;
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
   }


   //////////////////////////////////
   //	BUG
   //////////////////////////////////
   while(1) {}
}
