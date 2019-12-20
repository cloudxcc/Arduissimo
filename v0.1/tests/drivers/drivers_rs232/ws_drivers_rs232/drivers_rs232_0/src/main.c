//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_rs232.h"

int main(void)
{
   // INIT TX
   rs232_tx_init(0, 8, RS232_parity_even, RS232_stop_1, RS232_baud_115200);

   // INIT RX
   rs232_rx_init(0, 9, RS232_parity_even, RS232_baud_115200);

   // TX
   rs232_tx(0, 0x5A);

   // RX is handled in rs232_rx_finish

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
