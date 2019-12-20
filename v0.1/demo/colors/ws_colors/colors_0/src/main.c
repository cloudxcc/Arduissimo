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
	    GPIO_10_OUT_CLR = 0x40;
		//for (int i = 0; i < (CYCLE / 2 - (0 * CYCLE / 64)); i += 1)
	    //   GPIO_0_OUT_CLR = 0x01;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_10_OUT_SET = 0x40;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_10_OUT_CLR = 0x40;
		}
	}
}

void gpio_1(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_1_label >> 1) & 0x3ff);
	} else
	{
		gpio_1_label:
	    GPIO_10_OUT_CLR = 0x80;
		for (int i = 0; i < (1 * CYCLE / 12); i += 1)
	       GPIO_10_OUT_CLR = 0x80;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_10_OUT_SET = 0x80;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_10_OUT_CLR = 0x80;
		}
	}
}
void gpio_2(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_2_label >> 1) & 0x3ff);
	} else
	{
		gpio_2_label:
	    GPIO_11_OUT_CLR = 0x01;
		for (int i = 0; i < (2 * CYCLE / 12); i += 1)
	       GPIO_11_OUT_CLR = 0x01;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_SET = 0x01;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_CLR = 0x01;
		}
	}
}
int main(void)
{
	GPIO_10_DIR_SET = 0xc0;
	GPIO_11_DIR_SET = 0x01;

	int CT = CA_CT;
	gpio_0(CT);
	gpio_1(CT);
	gpio_2(CT);

	TC_KILL = 0x1;
}
