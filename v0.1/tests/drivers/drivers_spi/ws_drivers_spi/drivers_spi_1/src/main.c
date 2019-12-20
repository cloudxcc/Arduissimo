//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_spi.h"

int main(void)
{
   //////////////
   // SPI
   //////////////
   // INIT slave
   spi_slave_init(0, 12, 13, 14, 15, 3);

   while (1)
   {
   	if (GPIO_1_IN & 0x10) break;
   }

   // slave enable, write 0x7e
   spi_slave_enable(0, 0x7e);

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
