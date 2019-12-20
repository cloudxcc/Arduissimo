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
	    GPIO_11_OUT_CLR = 0x80;
		for (int i = 0; i < (9 * CYCLE / 12); i += 1)
	       GPIO_11_OUT_CLR = 0x80;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_SET = 0x80;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_11_OUT_CLR = 0x80;
		}
	}
}

void gpio_1(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_1_label >> 1) & 0x3ff);
	} else
	{
		gpio_1_label:
	    GPIO_12_OUT_CLR = 0x01;
		for (int i = 0; i < (10 * CYCLE / 12); i += 1)
	       GPIO_12_OUT_CLR = 0x01;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_12_OUT_SET = 0x01;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_12_OUT_CLR = 0x01;
		}
	}
}
void gpio_2(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_2_label >> 1) & 0x3ff);
	} else
	{
		gpio_2_label:
	    GPIO_12_OUT_CLR = 0x02;
		for (int i = 0; i < (11 * CYCLE / 12); i += 1)
	       GPIO_12_OUT_CLR = 0x02;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_12_OUT_SET = 0x02;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_12_OUT_CLR = 0x02;
		}
	}
}
int main(void)
{
	GPIO_11_DIR_SET = 0x80;
	GPIO_12_DIR_SET = 0x03;

	int CT = CA_CT;
	gpio_0(CT);
	gpio_1(CT);
	gpio_2(CT);

	TC_KILL = 0x1;
}
