//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"
#include "driver_custom.h"
#include "driver_pwm.h"

int main(void)
{
   //////////////
   // PWM
   //////////////
   // INIT
   pwm_init(0, 9, 500, 500);
   pwm_set(0, 1500, 500);

   TC_KILL = 0x0;
   asm volatile ("addi x0, x0, 0");
}
