//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "device.h"

#define CYCLE 100

void gpio_0(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_0_label >> 1) & 0x3ff);
	} else
	{
		gpio_0_label:
	    GPIO_4_OUT_CLR = 0x80;
		for (int i = 0; i < (32 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x80;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x80;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x80;
		}
	}
}

void gpio_1(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_1_label >> 1) & 0x3ff);
	} else
	{
		gpio_1_label:
	    GPIO_4_OUT_CLR = 0x40;
		for (int i = 0; i < (33 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x40;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x40;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x40;
		}
	}
}
void gpio_2(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_2_label >> 1) & 0x3ff);
	} else
	{
		gpio_2_label:
	    GPIO_4_OUT_CLR = 0x20;
		for (int i = 0; i < (34 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x20;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x20;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x20;
		}
	}
}
void gpio_3(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_3_label >> 1) & 0x3ff);
	} else
	{
		gpio_3_label:
	    GPIO_4_OUT_CLR = 0x10;
		for (int i = 0; i < (35 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x10;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x10;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x10;
		}
	}
}
void gpio_4(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_4_label >> 1) & 0x3ff);
	} else
	{
		gpio_4_label:
	    GPIO_4_OUT_CLR = 0x08;
		for (int i = 0; i < (36 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x08;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x08;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x08;
		}
	}
}
void gpio_5(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_5_label >> 1) & 0x3ff);
	} else
	{
		gpio_5_label:
	    GPIO_4_OUT_CLR = 0x04;
		for (int i = 0; i < (37 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x04;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x04;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x04;
		}
	}
}
void gpio_6(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_6_label >> 1) & 0x3ff);
	} else
	{
		gpio_6_label:
	    GPIO_4_OUT_CLR = 0x02;
		for (int i = 0; i < (38 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x02;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x02;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x02;
		}
	}
}
void gpio_7(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_7_label >> 1) & 0x3ff);
	} else
	{
		gpio_7_label:
	    GPIO_4_OUT_CLR = 0x01;
		for (int i = 0; i < (39 * CYCLE / 64); i += 1)
	       GPIO_4_OUT_CLR = 0x01;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_SET = 0x01;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_4_OUT_CLR = 0x01;
		}
	}
}
void gpio_8(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_8_label >> 1) & 0x3ff);
	} else
	{
		gpio_8_label:
	    GPIO_3_OUT_CLR = 0x01;
		for (int i = 0; i < (41 * CYCLE / 64); i += 1)
	       GPIO_3_OUT_CLR = 0x01;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_SET = 0x01;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_CLR = 0x01;
		}
	}
}
void gpio_9(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_9_label >> 1) & 0x3ff);
	} else
	{
		gpio_9_label:
	    GPIO_3_OUT_CLR = 0x10;
		for (int i = 0; i < (42 * CYCLE / 64); i += 1)
	       GPIO_3_OUT_CLR = 0x10;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_SET = 0x10;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_CLR = 0x10;
		}
	}
}
void gpio_10(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_10_label >> 1) & 0x3ff);
	} else
	{
		gpio_10_label:
	    GPIO_3_OUT_CLR = 0x02;
		for (int i = 0; i < (43 * CYCLE / 64); i += 1)
	       GPIO_3_OUT_CLR = 0x02;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_SET = 0x02;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_CLR = 0x02;
		}
	}
}
void gpio_11(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_11_label >> 1) & 0x3ff);
	} else
	{
		gpio_11_label:
	    GPIO_3_OUT_CLR = 0x20;
		for (int i = 0; i < (44 * CYCLE / 64); i += 1)
	       GPIO_3_OUT_CLR = 0x20;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_SET = 0x20;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_CLR = 0x20;
		}
	}
}
void gpio_12(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_12_label >> 1) & 0x3ff);
	} else
	{
		gpio_12_label:
	    GPIO_3_OUT_CLR = 0x04;
		for (int i = 0; i < (45 * CYCLE / 64); i += 1)
	       GPIO_3_OUT_CLR = 0x04;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_SET = 0x04;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_CLR = 0x04;
		}
	}
}
void gpio_13(int start_time) {
	if (start_time >= 0) {
		TC_START = 0xc0000000 + (((unsigned)&&gpio_13_label >> 1) & 0x3ff);
	} else
	{
		gpio_13_label:
	    GPIO_3_OUT_CLR = 0x40;
		for (int i = 0; i < (46 * CYCLE / 64); i += 1)
	       GPIO_3_OUT_CLR = 0x40;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_SET = 0x40;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_CLR = 0x40;
		}
	}
}
void gpio_14(int start_time) {
	if (start_time >= 0) {
		//TC_START = 0xc0000000 + (((unsigned)&&gpio_14_label >> 1) & 0x3ff);
	      CA_COM = 0xc0000000 +                                 // execute threat
	               (((unsigned)&&gpio_14_label >> 1) & 0x3fff); // set pc
	      CA_ET = start_time;
	} else
	{
		gpio_14_label:
	    GPIO_3_OUT_CLR = 0x08;
		for (int i = 0; i < (47 * CYCLE / 64); i += 1)
	       GPIO_3_OUT_CLR = 0x08;
	    while (1)
		{
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_SET = 0x08;
			for (int i = 0; i < CYCLE / 2; i += 1)
		       GPIO_3_OUT_CLR = 0x08;
		}
	}
}
int main(void)
{
	GPIO_3_DIR_SET = 0xff;
	GPIO_4_DIR_SET = 0xff;
	int CT = CA_CT;
	gpio_0(CT);
	gpio_1(CT);
	gpio_2(CT);
	gpio_3(CT);
    gpio_4(CT);
	gpio_5(CT);
	gpio_6(CT);
	gpio_7(CT);
    gpio_8(CT);
	gpio_9(CT);
	gpio_10(CT);
	gpio_11(CT);
	gpio_12(CT);
	gpio_13(CT);
	gpio_14(CT + 500);
	for (int i = 0; i < (47 * CYCLE / 64); i += 1)
       GPIO_3_OUT_CLR = 0x80;
	while (1)
	{
		for (int i = 0; i < CYCLE / 2; i += 1)
	       GPIO_3_OUT_SET = 0x80;
		for (int i = 0; i < CYCLE / 2; i += 1)
	       GPIO_3_OUT_CLR = 0x80;
	}
}
