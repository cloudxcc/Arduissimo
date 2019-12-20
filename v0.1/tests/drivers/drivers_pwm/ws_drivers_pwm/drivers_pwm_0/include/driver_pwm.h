//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#ifndef DRIVER_PWM_H
#define DRIVER_PWM_H

unsigned pwm_state [pwm_vp];
unsigned pwm_low [pwm_vp];
unsigned pwm_high [pwm_vp];
unsigned pwm_trigger [pwm_vp];
unsigned pwm_clr_com [pwm_vp];
unsigned pwm_set_com [pwm_vp];

void pwm_init(unsigned id, char pwm_pin, unsigned high, unsigned low);
void pwm_event(unsigned tag, int start_time);
void pwm_set(unsigned tag, unsigned high, unsigned low);

#endif
