//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

void uart_tx_init(unsigned tag, int start_time) {
   if (start_time >= 0) {
      UART_TX_COM = 0x80000000 + (((unsigned)&&uart_tx_label >> 1) & 0x3fff);
   } else
   {
      int i;
      uart_tx_label:
         GPIO_3_OUT_SET = 0xff;
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

void uart_rx(unsigned tag, int start_time) {
   if (start_time >= 0) {
      UART_RX_COM = 0x80000000 + (((unsigned)&&uart_rx_label >> 1) & 0x3fff);
   } else
   {
      int i;
      uart_rx_label:
         GPIO_2_OUT_SET = 0x1 << tag;
         TC_KILL = 0x0;
         asm volatile ("addi x0, x0, 0");
   }
}

int main(void)
{
   GPIO_0_DIR_SET = 0xff;
   GPIO_1_DIR_SET = 0xff;
   GPIO_2_DIR_SET = 0xff;
   GPIO_3_DIR_SET = 0xff;

   // just for synchronization
   for (int i = 0; i < 10; i++)
   {
      GPIO_0_OUT_SET = 0x1;
      GPIO_0_OUT_CLR = 0x1;
   }

   // read from UART
   for (int i = 0; i < 8; i++)
   {
      unsigned read = UART_REC;
      GPIO_1_OUT_SET = 0x1 << read;
   }

   // write to UART
   UART_SEND = 0xA5;
   int CT = CA_CT;
   uart_tx_init(0x0, CT);
   UART_SEND = 0x5A;

   // read from UART
   uart_rx(0x0, CT);

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
