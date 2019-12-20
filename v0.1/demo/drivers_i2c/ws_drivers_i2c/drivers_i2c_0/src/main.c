//
// Copyright 2019 Tobias Strauch, Munich, Bavaria
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_i2c.h"

int main(void)
{
   //////////////
   // I2C
   //////////////
   // INIT master
   i2c_master_init(0,  0,  1, I2C_baud_50k);
   i2c_master_init(1,  4,  5, I2C_baud_50k);
   i2c_master_init(2,  8,  9, I2C_baud_50k);
   i2c_master_init(3, 12, 13, I2C_baud_50k);
   i2c_master_init(4, 16, 17, I2C_baud_50k);
   i2c_master_init(5, 20, 21, I2C_baud_50k);
   i2c_master_init(6, 24, 25, I2C_baud_50k);
   i2c_master_init(7, 28, 29, I2C_baud_50k);

   // Init slave and enable
   i2c_slave_init(0,  2,  3, 0x40);
   i2c_slave_init(1,  6,  7, 0x41);
   i2c_slave_init(2, 10, 11, 0x42);
   i2c_slave_init(3, 14, 15, 0x43);
   i2c_slave_init(4, 18, 19, 0x44);
   i2c_slave_init(5, 22, 23, 0x45);
   i2c_slave_init(6, 26, 27, 0x46);
   i2c_slave_init(7, 30, 31, 0x47);
   i2c_slave_enable(0, 0);
   i2c_slave_enable(1, 0);
   i2c_slave_enable(2, 0);
   i2c_slave_enable(3, 0);
   i2c_slave_enable(4, 0);
   i2c_slave_enable(5, 0);
   i2c_slave_enable(6, 0);
   i2c_slave_enable(7, 0);

   // master write
   i2c_master_write(0, 0x40, 0x0);

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
