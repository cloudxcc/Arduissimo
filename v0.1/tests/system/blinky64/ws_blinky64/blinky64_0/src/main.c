//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

#define CYCLE 1000
#define OFFSET CYCLE / 64

void gpio0(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_0_OUT_SET = 0x1 << tag;
GPIO_0_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
     GPIO_0_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
     GPIO_0_OUT_CLR = 0x1 << tag;
     if (pwm == CYCLE)
         dir = -1;
     else if (pwm == 0)
         dir = 1;
     pwm += dir;
     t = 0;
   } else
     t += 1;
};
}

void gpio1(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_1_OUT_SET = 0x1 << tag;
GPIO_1_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
     GPIO_1_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
     GPIO_1_OUT_CLR = 0x1 << tag;
     if (pwm == CYCLE)
         dir = -1;
     else if (pwm == 0)
         dir = 1;
     pwm += dir;
     t = 0;
   } else
     t += 1;
};
}

void gpio0_0(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_0_label >> 1) & 0x3ff);
   } else
   {
     gpio0_0_label:
         gpio0(0, 0);
   }
}

void gpio0_1(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_1_label >> 1) & 0x3ff);
   } else
   {
     gpio0_1_label:
         gpio0(1, 1);
   }
}

void gpio0_2(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_2_label >> 1) & 0x3ff);
   } else
   {
     gpio0_2_label:
         gpio0(2, 2);
   }
}

void gpio0_3(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_3_label >> 1) & 0x3ff);
   } else
   {
     gpio0_3_label:
         gpio0(3, 3);
   }
}

void gpio0_4(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_4_label >> 1) & 0x3ff);
   } else
   {
     gpio0_4_label:
         gpio0(4, 4);
   }
}

void gpio0_5(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_5_label >> 1) & 0x3ff);
   } else
   {
     gpio0_5_label:
         gpio0(5, 5);
   }
}

void gpio0_6(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_6_label >> 1) & 0x3ff);
   } else
   {
     gpio0_6_label:
         gpio0(6, 6);
   }
}

void gpio0_7(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio0_7_label >> 1) & 0x3ff);
   } else
   {
     gpio0_7_label:
         gpio0(7, 7);
   }
}

void gpio1_0(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio1_0_label >> 1) & 0x3ff);
   } else
   {
     gpio1_0_label:
         gpio1(0, 8);
   }
}

void gpio1_1(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio1_1_label >> 1) & 0x3ff);
   } else
   {
     gpio1_1_label:
         gpio1(1, 9);
   }
}

void gpio1_2(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio1_2_label >> 1) & 0x3ff);
   } else
   {
     gpio1_2_label:
         gpio1(2, 10);
   }
}

void gpio1_3(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio1_3_label >> 1) & 0x3ff);
   } else
   {
     gpio1_3_label:
         gpio1(3, 11);
   }
}

void gpio1_4(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio1_4_label >> 1) & 0x3ff);
   } else
   {
     gpio1_4_label:
         gpio1(4, 12);
   }
}

void gpio1_5(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio1_5_label >> 1) & 0x3ff);
   } else
   {
     gpio1_5_label:
         gpio1(5, 13);
   }
}

void gpio1_6(int start_time) {
   if (start_time >= 0) {
     TC_START = 0xc0000000 + (((unsigned)&&gpio1_6_label >> 1) & 0x3ff);
   } else
   {
     gpio1_6_label:
         gpio1(6, 14);
   }
}

int main(void)
{
   GPIO_0_DIR_SET = 0xff;
   GPIO_1_DIR_SET = 0xff;

   int CT = CA_CT;
   gpio0_0(CT);
   gpio0_1(CT);
   gpio0_2(CT);
   gpio0_3(CT);
   gpio0_4(CT);
   gpio0_5(CT);
   gpio0_6(CT);
   gpio0_7(CT);
   gpio1_0(CT);
   gpio1_1(CT);
   gpio1_2(CT);
   gpio1_3(CT);
   gpio1_4(CT);
   gpio1_5(CT);
   gpio1_6(CT);
   gpio1(7, 15);
}
