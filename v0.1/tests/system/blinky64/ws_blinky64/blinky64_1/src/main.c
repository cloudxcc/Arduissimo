//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

#define CYCLE 1000
#define OFFSET CYCLE / 64

void gpio2(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_2_OUT_SET = 0x1 << tag;
GPIO_2_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
      GPIO_2_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
      GPIO_2_OUT_CLR = 0x1 << tag;
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

void gpio3(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_3_OUT_SET = 0x1 << tag;
GPIO_3_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
      GPIO_3_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
      GPIO_3_OUT_CLR = 0x1 << tag;
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

void gpio2_0(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_0_label >> 1) & 0x3ff);
   } else
   {
      gpio2_0_label:
         gpio2(0, 16);
   }
}

void gpio2_1(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_1_label >> 1) & 0x3ff);
   } else
   {
      gpio2_1_label:
         gpio2(1, 17);
   }
}

void gpio2_2(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_2_label >> 1) & 0x3ff);
   } else
   {
      gpio2_2_label:
         gpio2(2, 18);
   }
}

void gpio2_3(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_3_label >> 1) & 0x3ff);
   } else
   {
      gpio2_3_label:
         gpio2(3, 19);
   }
}

void gpio2_4(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_4_label >> 1) & 0x3ff);
   } else
   {
      gpio2_4_label:
         gpio2(4, 20);
   }
}

void gpio2_5(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_5_label >> 1) & 0x3ff);
   } else
   {
      gpio2_5_label:
         gpio2(5, 21);
   }
}

void gpio2_6(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_6_label >> 1) & 0x3ff);
   } else
   {
      gpio2_6_label:
         gpio2(6, 22);
   }
}

void gpio2_7(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio2_7_label >> 1) & 0x3ff);
   } else
   {
      gpio2_7_label:
         gpio2(7, 23);
   }
}

void gpio3_0(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio3_0_label >> 1) & 0x3ff);
   } else
   {
      gpio3_0_label:
         gpio3(0, 24);
   }
}

void gpio3_1(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio3_1_label >> 1) & 0x3ff);
   } else
   {
      gpio3_1_label:
         gpio3(1, 25);
   }
}

void gpio3_2(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio3_2_label >> 1) & 0x3ff);
   } else
   {
      gpio3_2_label:
         gpio3(2, 26);
   }
}

void gpio3_3(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio3_3_label >> 1) & 0x3ff);
   } else
   {
      gpio3_3_label:
         gpio3(3, 27);
   }
}

void gpio3_4(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio3_4_label >> 1) & 0x3ff);
   } else
   {
      gpio3_4_label:
         gpio3(4, 28);
   }
}

void gpio3_5(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio3_5_label >> 1) & 0x3ff);
   } else
   {
      gpio3_5_label:
         gpio3(5, 29);
   }
}

void gpio3_6(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio3_6_label >> 1) & 0x3ff);
   } else
   {
      gpio3_6_label:
         gpio3(6, 30);
   }
}

int main(void)
{
   GPIO_2_DIR_SET = 0xff;
   GPIO_3_DIR_SET = 0xff;

   int CT = CA_CT;
   gpio2_0(CT);
   gpio2_1(CT);
   gpio2_2(CT);
   gpio2_3(CT);
   gpio2_4(CT);
   gpio2_5(CT);
   gpio2_6(CT);
   gpio2_7(CT);
   gpio3_0(CT);
   gpio3_1(CT);
   gpio3_2(CT);
   gpio3_3(CT);
   gpio3_4(CT);
   gpio3_5(CT);
   gpio3_6(CT);
   gpio3(7, 31);
}
