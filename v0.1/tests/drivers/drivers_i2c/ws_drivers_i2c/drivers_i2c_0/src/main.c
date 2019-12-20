//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_i2c.h"

int main(void)
{
   //////////////
   // reset program state
   //////////////
   program_state = 0;

   //////////////
   // I2C
   //////////////
   // INIT master
   i2c_master_init(0, 10, 11, 2000);

   // master write to "wrong" add 0x42
   i2c_master_write(0, 0x42, 0x5a);

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
