//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_rs232.h"

unsigned gl_tx_cnt;

int main(void)
{
   gl_tx_cnt = 0;
   // INIT TX
   rs232_tx_init(0, 32, RS232_parity_even, RS232_stop_1, RS232_baud_115200);
   rs232_tx_init(1, 33, RS232_parity_even, RS232_stop_1, RS232_baud_115200);
   rs232_tx_init(2, 34, RS232_parity_even, RS232_stop_1, RS232_baud_115200);
   rs232_tx_init(3, 35, RS232_parity_even, RS232_stop_1, RS232_baud_115200);
   rs232_tx_init(4, 36, RS232_parity_even, RS232_stop_1, RS232_baud_115200);
   rs232_tx_init(5, 37, RS232_parity_even, RS232_stop_1, RS232_baud_115200);
   rs232_tx_init(6, 38, RS232_parity_even, RS232_stop_1, RS232_baud_115200);
   rs232_tx_init(7, 39, RS232_parity_even, RS232_stop_1, RS232_baud_115200);

   // INIT RX
   rs232_rx_init(0, 32, RS232_parity_even, RS232_baud_115200);
   rs232_rx_init(1, 33, RS232_parity_even, RS232_baud_115200);
   rs232_rx_init(2, 34, RS232_parity_even, RS232_baud_115200);
   rs232_rx_init(3, 35, RS232_parity_even, RS232_baud_115200);
   rs232_rx_init(4, 36, RS232_parity_even, RS232_baud_115200);
   rs232_rx_init(5, 37, RS232_parity_even, RS232_baud_115200);
   rs232_rx_init(6, 38, RS232_parity_even, RS232_baud_115200);

   // TX
   rs232_tx(0, 0x00);

   // RX is handled in rs232_rx_finish

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
