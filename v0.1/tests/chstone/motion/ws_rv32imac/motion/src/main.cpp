//
// Copyright 2019 Tobias Strauch, Munich, Bavaria, Germany
// Licensed under the Apache License, Version 2.0, see LICENSE for details.
// SPDX-License-Identifier: Apache-2.0
//

#include "mpeg2.c"
#include "mpeg2dec.h"

int evalue;
#include "config.h"
#include "global.h"
/*
#include "getbits.c"
*/

/*
unsigned* tb = (unsigned*)0x1001200c;
unsigned* dbg0 = (unsigned*)0x10012004;
unsigned* dbg1 = (unsigned*)0x10012008;
*/

int
read (unsigned char *s1, const unsigned char *s2, int n)
{
  unsigned char *p1;
  const unsigned char *p2;
  int n_tmp;
  p1 = s1;
  p2 = s2;
  n_tmp = n;
  while (n_tmp-- > 0)
   {
     *p1 = *p2;
     p1++;
     p2++;
   }
  return n;
}
void
Fill_Buffer ()
{
  int Buffer_Level;
  unsigned char *p;
  p = ld_Rdbfr;

  unsigned* dbg1 = (unsigned*)0x10012008;
  *dbg1 = 0xaffe00bb;
  unsigned* dbg0 = (unsigned*)0x10012004;
  *dbg0 = (unsigned)ld_Rdbfr;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");

  Buffer_Level = read (ld_Rdbfr, inRdbfr, 2048);
  ld_Rdptr = ld_Rdbfr;

  if (System_Stream_Flag)
   ld_Rdmax -= 2048;

  /* end of the bitstream file */
  if (Buffer_Level < 2048)
   {
     /* just to be safe */
     if (Buffer_Level < 0)
   Buffer_Level = 0;

     /* pad until the next to the next 32-bit word boundary */
     while (Buffer_Level & 3)
   ld_Rdbfr[Buffer_Level++] = 0;

     /* pad the buffer with sequence end codes */
     while (Buffer_Level < 2048)
   {
     ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE >> 24;
     ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE >> 16;
     ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE >> 8;
     ld_Rdbfr[Buffer_Level++] = SEQUENCE_END_CODE & 0xff;
   }
   }
}

unsigned int
Show_Bits (int N)
{
  return ld_Bfr >> (unsigned)(32-N)%32;
}

/* return next bit (could be made faster than Get_Bits(1)) */
unsigned int
Get_Bits1 ()
{
  return Get_Bits (1);
}

/* advance by n bits */
void
Flush_Buffer (int N)
{
  int Incnt;

  ld_Bfr <<= N;

  Incnt = ld_Incnt -= N;

  unsigned* dbg1 = (unsigned*)0x10012008;
  *dbg1 = 0xaffe00aa;
  unsigned* dbg0 = (unsigned*)0x10012004;
  *dbg0 = Incnt;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");

  if (Incnt <= 24)
  {
   if (ld_Rdptr < ld_Rdbfr + 2044)
   {
   	  *dbg0 = 0x1;
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
     do
      {
        ld_Bfr |= *ld_Rdptr++ << (24 - Incnt);
        Incnt += 8;
      }
     while (Incnt <= 24);
   }
   else
   {
     if (ld_Rdptr >= (ld_Rdbfr + 2048))
        Fill_Buffer ();

     do
      {
   	  *dbg0 = (unsigned)ld_Rdptr;
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");

   	  *dbg0 = (unsigned)ld_Rdbfr;
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");

   	  /*
   	  if (ld_Rdptr >= (ld_Rdbfr + 2048))
        Fill_Buffer ();
   	  */

   	  *dbg0 = *ld_Rdptr;
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");

   	  ld_Bfr |= *ld_Rdptr++ << (24 - Incnt);

   	  *dbg0 = ld_Bfr;
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");
   	  asm volatile ("addi x0, x0, 0");

        Incnt += 8;
      }
     while (Incnt <= 24);
   }
     ld_Incnt = Incnt;
   }
  *dbg1 = 0xaffe00a0;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
}


/* return next n bits (right adjusted) */
unsigned int
Get_Bits (int N)
{
  unsigned int Val;

  Val = Show_Bits (N);
  Flush_Buffer (N);

  return Val;
}

//#include "getvlc.h"
/* Table B-10, motion_code, codes 0001 ... 01xx */
const char MVtab0[8][2] = {
  {ERROR, 0}, {3, 3}, {2, 2}, {2, 2},
  {1, 1}, {1, 1}, {1, 1}, {1, 1}
};

/* Table B-10, motion_code, codes 0000011 ... 000011x */
const char MVtab1[8][2] = {
  {ERROR, 0}, {ERROR, 0}, {ERROR, 0}, {7, 6},
  {6, 6}, {5, 6}, {4, 5}, {4, 5}
};

/* Table B-10, motion_code, codes 0000001100 ... 000001011x */
const char MVtab2[12][2] = {
  {16, 9}, {15, 9}, {14, 9}, {13, 9}, {12, 9}, {11, 9},
  {10, 8}, {10, 8}, {9, 8}, {9, 8}, {8, 8}, {8, 8}
};

//#include "getvlc.c"
int Get_motion_code ()
{
  int code;

  if (Get_Bits1 ())
   {
     return 0;
   }

  code = Show_Bits (9);

  unsigned* dbg1 = (unsigned*)0x10012008;
  *dbg1 = 0xaffe0000;
  unsigned* dbg0 = (unsigned*)0x10012004;
  *dbg0 = code;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");

  if (code >= 64)
   {
     code >>= 6;
     Flush_Buffer (MVtab0[code][1]);

     return Get_Bits1 ()? -MVtab0[code][0] : MVtab0[code][0];
   }

  if (code >= 24)
   {
     code >>= 3;
     Flush_Buffer (MVtab1[code][1]);

     return Get_Bits1 ()? -MVtab1[code][0] : MVtab1[code][0];
   }

  code -= 12;
  if (code < 0)
   return 0;

  *dbg0 = code;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");

  Flush_Buffer (MVtab2[code][1]);
  return Get_Bits1 ()? -MVtab2[code][0] : MVtab2[code][0];
}
/* get differential motion vector (for dual prime prediction) */
int
Get_dmvector ()
{

  if (Get_Bits (1))
   {
     return Get_Bits (1) ? -1 : 1;
   }
  else
   {
     return 0;
   }
}

/*
#include "motion.c"
*/
void
motion_vectors (int PMV[2][2][2], int dmvector[2], int motion_vertical_field_select[2][2], int s,
   	int motion_vector_count, int mv_format, int h_r_size, int v_r_size, int dmv,
   	int mvscale)
    //int PMV[2][2][2];
    //int dmvector[2];
    //int motion_vertical_field_select[2][2];
    //int s, motion_vector_count, mv_format, h_r_size, v_r_size, dmv, mvscale;
{
  if (motion_vector_count == 1)
   {
     if (mv_format == MV_FIELD && !dmv)
   {
     motion_vertical_field_select[1][s] =
      motion_vertical_field_select[0][s] = Get_Bits (1);
   }

     motion_vector (PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);

     unsigned* dbg1 = (unsigned*)0x10012008;
     *dbg1 = 0xaffedead; //PMV[i][j][k];
     unsigned* dbg0 = (unsigned*)0x10012004;
     *dbg0 = PMV[0][0][0];

     /* update other motion vector predictors */
     PMV[1][s][0] = PMV[0][s][0];
     PMV[1][s][1] = PMV[0][s][1];
   }
  else
   {
     motion_vertical_field_select[0][s] = Get_Bits (1);

     motion_vector (PMV[0][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);

     motion_vertical_field_select[1][s] = Get_Bits (1);

     motion_vector (PMV[1][s], dmvector, h_r_size, v_r_size, dmv, mvscale, 0);
   }
}
void decode_motion_vector (int *pred, int r_size, int motion_code, int motion_residual,
   	int full_pel_vector)
{
  int lim, vec;

  unsigned* dbg1 = (unsigned*)0x10012008;
  *dbg1 = 0xaffe0001; //PMV[i][j][k];
  unsigned* dbg0 = (unsigned*)0x10012004;
  *dbg0 = motion_code;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  *dbg0 = full_pel_vector;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  *dbg0 = r_size;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  *dbg0 = motion_residual;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  *dbg0 = *pred;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");

  r_size = r_size % 32;
  lim = 16 << r_size;
  *dbg0 = lim;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");

  vec = full_pel_vector ? (*pred >> 1) : (*pred);

  *dbg0 = vec;
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");
  asm volatile ("addi x0, x0, 0");

  if (motion_code > 0)
   {
     vec += ((motion_code - 1) << r_size) + motion_residual + 1;
     *dbg0 = vec;
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");

     if (vec >= lim)
   vec -= lim + lim;
   }
  else if (motion_code < 0)
   {
     vec -= ((-motion_code - 1) << r_size) + motion_residual + 1;
     if (vec < -lim)
   vec += lim + lim;
   }
  *pred = full_pel_vector ? (vec << 1) : vec;

  *dbg0 = *pred;

}
/* get and decode motion vector and differential motion vector
   for one prediction */
void
motion_vector (int *PMV, int *dmvector, int h_r_size, int v_r_size, int dmv, int mvscale,
   	int full_pel_vector)
/*    int *PMV;
    int *dmvector;
    int h_r_size;
    int v_r_size;
    int dmv;      	/* MPEG-2 only: get differential motion vectors */
//    int mvscale;   	/* MPEG-2 only: field vector in frame pic */
//    int full_pel_vector;	/* MPEG-1 only */
{
  int motion_code;
  int motion_residual;

  /* horizontal component */
  /* ISO/IEC 13818-2 Table B-10 */
  motion_code = Get_motion_code ();

  motion_residual = (h_r_size != 0
        && motion_code != 0) ? Get_Bits (h_r_size) : 0;

  decode_motion_vector (&PMV[0], h_r_size, motion_code, motion_residual, full_pel_vector);

  if (dmv)
    dmvector[0] = Get_dmvector ();

  unsigned* dbg1 = (unsigned*)0x10012008;
  *dbg1 = 0xaffe0002; //PMV[i][j][k];
  unsigned* dbg0 = (unsigned*)0x10012004;
  *dbg0 = *PMV;

  /* vertical component */
  motion_code = Get_motion_code ();
  motion_residual = (v_r_size != 0
        && motion_code != 0) ? Get_Bits (v_r_size) : 0;

  if (mvscale)
   PMV[1] >>= 1;   	/* DIV 2 */

  decode_motion_vector (&PMV[1], v_r_size, motion_code, motion_residual, full_pel_vector);

  if (mvscale)
   PMV[1] <<= 1;

  if (dmv)
    dmvector[1] = Get_dmvector ();

}



bool debug = true;

void
Initialize_Buffer ()
{
   ld_Incnt = 0;
   ld_Rdptr = ld_Rdbfr + 2048;
   ld_Rdmax = ld_Rdptr;
   ld_Bfr = 68157440;
   Flush_Buffer (0);   	/* fills valid data into bfr */
     unsigned* dbg1 = (unsigned*)0x10012008;
     *dbg1 = 0xaffe000a;
     unsigned* dbg0 = (unsigned*)0x10012004;
     *dbg0 = ld_Bfr;
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");
     asm volatile ("addi x0, x0, 0");

}


int
main()
{


   unsigned* tb = (unsigned*)0x1001200c;
   unsigned* dbg0 = (unsigned*)0x10012004;
   unsigned* dbg1 = (unsigned*)0x10012008;

   int i, j, k;
   int main_result;
   int PMV[2][2][2];
   int dmvector[2];
   int motion_vertical_field_select[2][2];
   int s, motion_vector_count, mv_format, h_r_size, v_r_size, dmv, mvscale;

   while (1)
   {
      main_result = 0;
      evalue = 0;
      System_Stream_Flag = 0;
      s = 0;
      motion_vector_count = 1;
      mv_format = 0;
      h_r_size = 200;
      v_r_size = 200;
      dmv = 0;
      mvscale = 1;
      for (i = 0; i < 2; i++)
      {
      dmvector[i] = 0;
      *tb = dmvector[i];
      for (j = 0; j < 2; j++)
      {
          motion_vertical_field_select[i][j] = inmvfs[i][j];
          *tb = motion_vertical_field_select[i][j];
          for (k = 0; k < 2; k++)
          {
            PMV[i][j][k] = inPMV[i][j][k];
            *tb = PMV[i][j][k];
          }
      }
      }
      if (false)
      for (i = 0; i < 2; i++)
      {
      *tb = dmvector[i];
      for (j = 0; j < 2; j++)
      {
          *tb = motion_vertical_field_select[i][j];
          for (k = 0; k < 2; k++)
            *tb = PMV[i][j][k];
      }
      }

      //*tb = main_result;
      for (i = 0; i < 2; i++)
      for (j = 0; j < 2; j++)
      {
      //*tb = motion_vertical_field_select[i][j];
          //*tb = outmvfs[i][j];
      main_result += (motion_vertical_field_select[i][j] != inmvfs[i][j]);
          //*tb = main_result;
      for (k = 0; k < 2; k++)
      {
         *tb = PMV[i][j][k];
         *tb = inPMV[i][j][k];
         main_result += (PMV[i][j][k] != inPMV[i][j][k]);
         //*tb = main_result;
      }
      }

      Initialize_Buffer ();

      for (i = 0; i < 2; i++)
      {
      *tb = dmvector[i];
      for (j = 0; j < 2; j++)
      {
          *tb = motion_vertical_field_select[i][j];
          for (k = 0; k < 2; k++)
            *tb = PMV[i][j][k];
      }
      }

      //if (dmvector[0] != 1)
      *tb = -2;
      *tb = 0x80001;

      motion_vectors (PMV, dmvector, motion_vertical_field_select, s,
         motion_vector_count, mv_format, h_r_size, v_r_size, dmv,
         mvscale);

      //*tb = main_result;
      for (i = 0; i < 2; i++)
      for (j = 0; j < 2; j++)
      {
      //*tb = motion_vertical_field_select[i][j];
          //*tb = outmvfs[i][j];
      main_result += (motion_vertical_field_select[i][j] != outmvfs[i][j]);
          //*tb = main_result;
      for (k = 0; k < 2; k++)
      {
         *dbg0 = PMV[i][j][k];
         *dbg1 = outPMV[i][j][k];
         main_result += (PMV[i][j][k] != outPMV[i][j][k]);
         //*tb = main_result;
      }
      }


      if (main_result != 0x0)
      {
         *tb = 0x80000;
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         asm volatile ("addi x0, x0, 0");
         *tb = 0x80000;
         break;
      }

      *tb = 0x400000;
      asm volatile ("addi x0, x0, 0");
      asm volatile ("addi x0, x0, 0");
      asm volatile ("addi x0, x0, 0");
      asm volatile ("addi x0, x0, 0");
      *tb = 0x400000;
      asm volatile ("addi x0, x0, 0");
      asm volatile ("addi x0, x0, 0");
      asm volatile ("addi x0, x0, 0");
      asm volatile ("addi x0, x0, 0");
   }


   //////////////////////////////////
   //	BUG
   //////////////////////////////////
   while(1) {}

return 0;
}
