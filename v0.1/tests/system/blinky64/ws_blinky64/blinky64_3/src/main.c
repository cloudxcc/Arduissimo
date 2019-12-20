//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

#define CYCLE 1000
#define OFFSET CYCLE / 64

void gpio6(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_6_OUT_SET = 0x1 << tag;
GPIO_6_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
      GPIO_6_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
      GPIO_6_OUT_CLR = 0x1 << tag;
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

void gpio7(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_7_OUT_SET = 0x1 << tag;
GPIO_7_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
      GPIO_7_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
      GPIO_7_OUT_CLR = 0x1 << tag;
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

void gpio6_0(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_0_label >> 1) & 0x3ff);
   } else
   {
      gpio6_0_label:
          gpio6(0, 48);
   }
}

void gpio6_1(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_1_label >> 1) & 0x3ff);
   } else
   {
      gpio6_1_label:
          gpio6(1, 49);
   }
}

void gpio6_2(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_2_label >> 1) & 0x3ff);
   } else
   {
      gpio6_2_label:
          gpio6(2, 50);
   }
}

void gpio6_3(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_3_label >> 1) & 0x3ff);
   } else
   {
      gpio6_3_label:
          gpio6(3, 51);
   }
}

void gpio6_4(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_4_label >> 1) & 0x3ff);
   } else
   {
      gpio6_4_label:
          gpio6(4, 52);
   }
}

void gpio6_5(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_5_label >> 1) & 0x3ff);
   } else
   {
      gpio6_5_label:
          gpio6(5, 53);
   }
}

void gpio6_6(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_6_label >> 1) & 0x3ff);
   } else
   {
      gpio6_6_label:
          gpio6(6, 54);
   }
}

void gpio6_7(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio6_7_label >> 1) & 0x3ff);
   } else
   {
      gpio6_7_label:
          gpio6(7, 55);
   }
}

void gpio7_0(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio7_0_label >> 1) & 0x3ff);
   } else
   {
      gpio7_0_label:
          gpio7(0, 56);
   }
}

void gpio7_1(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio7_1_label >> 1) & 0x3ff);
   } else
   {
      gpio7_1_label:
          gpio7(1, 57);
   }
}

void gpio7_2(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio7_2_label >> 1) & 0x3ff);
   } else
   {
      gpio7_2_label:
          gpio7(2, 58);
   }
}

void gpio7_3(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio7_3_label >> 1) & 0x3ff);
   } else
   {
      gpio7_3_label:
          gpio7(3, 59);
   }
}

void gpio7_4(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio7_4_label >> 1) & 0x3ff);
   } else
   {
      gpio7_4_label:
          gpio7(4, 60);
   }
}

void gpio7_5(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio7_5_label >> 1) & 0x3ff);
   } else
   {
      gpio7_5_label:
          gpio7(5, 61);
   }
}

void gpio7_6(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio7_6_label >> 1) & 0x3ff);
   } else
   {
      gpio7_6_label:
          gpio7(6, 62);
   }
}

int main(void)
{
   GPIO_6_DIR_SET = 0xff;
   GPIO_7_DIR_SET = 0xff;

   int CT = CA_CT;
   gpio6_0(CT);
   gpio6_1(CT);
   gpio6_2(CT);
   gpio6_3(CT);
   gpio6_4(CT);
   gpio6_5(CT);
   gpio6_6(CT);
   gpio6_7(CT);
   gpio7_0(CT);
   gpio7_1(CT);
   gpio7_2(CT);
   gpio7_3(CT);
   gpio7_4(CT);
   gpio7_5(CT);
   gpio7_6(CT);
   gpio7(7, 63);
}
