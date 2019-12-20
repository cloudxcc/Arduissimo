//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "sha.h"
#include "sha.c"

/*
+--------------------------------------------------------------------------+
| * Test Vector (added for CHStone)                              |
|    outData : expected output data                              |
+--------------------------------------------------------------------------+
*/
const INT32 outData[5] =
{ 0x006a5a37UL, 0x93dc9485UL, 0x2c412112UL, 0x63f7ba43UL, 0xad73f922UL };

int
main ()
{
   int i;
   int main_result;

   unsigned* tb = (unsigned*)0x1001200c;

   while (1)
   {
      main_result = 0;
      sha_stream ();

      for (i = 0; i < 5; i++)
      {
      main_result += (sha_info_digest[i] != outData[i]);
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
