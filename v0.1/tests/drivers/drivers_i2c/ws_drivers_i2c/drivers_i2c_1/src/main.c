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
	// Init slave and enable
	i2c_slave_init(0, 12, 13, 0x41);
	i2c_slave_enable(0, 0x7e);

    TC_KILL = 0x0;
    asm volatile ("addi x0, x0, 0");
}
