//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

int main_result;

#include "aes.h"
#include "aes_enc.c"
#include "aes_dec.c"
#include "aes_key.c"
#include "aes_func.c"

/* ***************** main **************************** */
int
aes_main (void)
{
   /*
   +--------------------------------------------------------------------------+
   | * Test Vectors (added for CHStone)                              |
   |    statemt, key : input data                                 |
   +--------------------------------------------------------------------------+
   */
   statemt[0] = 50;
   statemt[1] = 67;
   statemt[2] = 246;
   statemt[3] = 168;
   statemt[4] = 136;
   statemt[5] = 90;
   statemt[6] = 48;
   statemt[7] = 141;
   statemt[8] = 49;
   statemt[9] = 49;
   statemt[10] = 152;
   statemt[11] = 162;
   statemt[12] = 224;
   statemt[13] = 55;
   statemt[14] = 7;
   statemt[15] = 52;

   key[0] = 43;
   key[1] = 126;
   key[2] = 21;
   key[3] = 22;
   key[4] = 40;
   key[5] = 174;
   key[6] = 210;
   key[7] = 166;
   key[8] = 171;
   key[9] = 247;
   key[10] = 21;
   key[11] = 136;
   key[12] = 9;
   key[13] = 207;
   key[14] = 79;
   key[15] = 60;

   encrypt (statemt, key, 128128);
   decrypt (statemt, key, 128128);
   return 0;
}


int main (void)
{
   unsigned* tb = (unsigned*)0x1001200c;

   while (1)
   {
      main_result = 0;
      aes_main ();

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
