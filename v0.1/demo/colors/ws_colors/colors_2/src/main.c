//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"

#define CYCLE 10000000

void gpio_0(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_0_label >> 1) & 0x3ff);
	} else
	{
		gpio_0_label:
	    GPIO_11_OUT_CLR = 0x10;
		for (int i = 0; i < (6 * CYCLE / 12); i += 1)
	       GPIO_11_OUT_CLR = 0x10;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_SET = 0x10;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_CLR = 0x10;
		}
	}
}

void gpio_1(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_1_label >> 1) & 0x3ff);
	} else
	{
		gpio_1_label:
	    GPIO_11_OUT_CLR = 0x20;
		for (int i = 0; i < (7 * CYCLE / 12); i += 1)
	       GPIO_11_OUT_CLR = 0x20;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_SET = 0x20;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_CLR = 0x20;
		}
	}
}
void gpio_2(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_2_label >> 1) & 0x3ff);
	} else
	{
		gpio_2_label:
	    GPIO_11_OUT_CLR = 0x40;
		for (int i = 0; i < (8 * CYCLE / 12); i += 1)
	       GPIO_11_OUT_CLR = 0x40;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_SET = 0x40;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_CLR = 0x40;
		}
	}
}
int main(void)
{
	GPIO_11_DIR_SET = 0x70;

	int CT = CA_CT;
	gpio_0(CT);
	gpio_1(CT);
	gpio_2(CT);

	TC_KILL = 0x1;
}
