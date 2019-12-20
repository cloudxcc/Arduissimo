//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//


int a_int;
int b_int;
int c_int;

unsigned a_unsigned;
unsigned b_unsigned;
unsigned c_unsigned;

int main (void)
{
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   asm volatile ("addi x0, x0, 0");
   
   unsigned* tb = (unsigned*)0x1001200c;

   //////////////////////////////////
   //	XORI 
   //////////////////////////////////   
   a_int = 0x33330000;
   a_int += 0x3333;
   c_int = a_int ^ 0x555;
   if (c_int != 0x33333666)
   {
       *tb = 0x80000;
   }
   *tb = 0x400000;
   *tb = 0x400000;
}
