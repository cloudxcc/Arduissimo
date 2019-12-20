//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
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
   // INIT master
   spi_master_init(0, 9, 10, 11, 3, 2000);
   spi_master_ss_init(0, 8);

   // master write 0x5a
   spi_master_write(0, 0, 0x5a);

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
