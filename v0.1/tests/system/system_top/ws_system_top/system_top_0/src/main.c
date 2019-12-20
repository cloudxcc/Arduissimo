//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

int main(void)
{

   UART_SEND = UART_REC;
   UART_SEND = UART_REC;

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
