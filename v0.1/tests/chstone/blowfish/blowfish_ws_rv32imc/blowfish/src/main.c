//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "blowfish.h"
#include "bf_locl.h"
#include "bf_pi.h"
#include "bf_skey.c"
#include "bf_cfb64.c"
#include "bf_enc.c"
#include "bf.c"

int
main ()
{
   unsigned* tb = (unsigned*)0x1001200c;

   int main_result;

   while (1)
   {
      main_result = 0;
      main_result = blowfish_main ();

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
