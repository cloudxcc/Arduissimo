//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "system.c"

#define CYCLE 1000
#define OFFSET CYCLE / 64

void gpio4(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_4_OUT_SET = 0x1 << tag;
GPIO_4_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
      GPIO_4_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
      GPIO_4_OUT_CLR = 0x1 << tag;
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

void gpio5(int tag, int thisOffset) {
int t, pwm, dir;
t = 0;
dir = 1;
pwm = OFFSET * thisOffset;
GPIO_5_OUT_SET = 0x1 << tag;
GPIO_5_OUT_CLR = 0x1 << tag;
while (1) {
   if (t == pwm)
      GPIO_5_OUT_SET = 0x1 << tag;
   if (t == CYCLE) {
      GPIO_5_OUT_CLR = 0x1 << tag;
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

void gpio4_0(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_0_label >> 1) & 0x3ff);
   } else
   {
      gpio4_0_label:
          gpio4(0, 32);
   }
}

void gpio4_1(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_1_label >> 1) & 0x3ff);
   } else
   {
      gpio4_1_label:
          gpio4(1, 33);
   }
}

void gpio4_2(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_2_label >> 1) & 0x3ff);
   } else
   {
      gpio4_2_label:
          gpio4(2, 34);
   }
}

void gpio4_3(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_3_label >> 1) & 0x3ff);
   } else
   {
      gpio4_3_label:
          gpio4(3, 35);
   }
}

void gpio4_4(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_4_label >> 1) & 0x3ff);
   } else
   {
      gpio4_4_label:
          gpio4(4, 36);
   }
}

void gpio4_5(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_5_label >> 1) & 0x3ff);
   } else
   {
      gpio4_5_label:
          gpio4(5, 37);
   }
}

void gpio4_6(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_6_label >> 1) & 0x3ff);
   } else
   {
      gpio4_6_label:
          gpio4(6, 38);
   }
}

void gpio4_7(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio4_7_label >> 1) & 0x3ff);
   } else
   {
      gpio4_7_label:
          gpio4(7, 39);
   }
}

void gpio5_0(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio5_0_label >> 1) & 0x3ff);
   } else
   {
      gpio5_0_label:
          gpio5(0, 40);
   }
}

void gpio5_1(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio5_1_label >> 1) & 0x3ff);
   } else
   {
      gpio5_1_label:
          gpio5(1, 41);
   }
}

void gpio5_2(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio5_2_label >> 1) & 0x3ff);
   } else
   {
      gpio5_2_label:
          gpio5(2, 42);
   }
}

void gpio5_3(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio5_3_label >> 1) & 0x3ff);
   } else
   {
      gpio5_3_label:
          gpio5(3, 43);
   }
}

void gpio5_4(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio5_4_label >> 1) & 0x3ff);
   } else
   {
      gpio5_4_label:
          gpio5(4, 44);
   }
}

void gpio5_5(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio5_5_label >> 1) & 0x3ff);
   } else
   {
      gpio5_5_label:
          gpio5(5, 45);
   }
}

void gpio5_6(int start_time) {
   if (start_time >= 0) {
      TC_START = 0xc0000000 + (((unsigned)&&gpio5_6_label >> 1) & 0x3ff);
   } else
   {
      gpio5_6_label:
          gpio5(6, 46);
   }
}

int main(void)
{
   GPIO_4_DIR_SET = 0xff;
   GPIO_5_DIR_SET = 0xff;

   int CT = CA_CT;
   gpio4_0(CT);
   gpio4_1(CT);
   gpio4_2(CT);
   gpio4_3(CT);
   gpio4_4(CT);
   gpio4_5(CT);
   gpio4_6(CT);
   gpio4_7(CT);
   gpio5_0(CT);
   gpio5_1(CT);
   gpio5_2(CT);
   gpio5_3(CT);
   gpio5_4(CT);
   gpio5_5(CT);
   gpio5_6(CT);
   gpio5(7, 47);
}
