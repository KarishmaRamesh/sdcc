#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "hw.h"
#include "serial.h"
#include "print.h"

#include "c517.h"

#include "mdu517.h"

__xdata char *simif;

unsigned long lop1, lop2, lres, mdu_lres;
unsigned int iop1, iop2, ires1, ires2, mdu_ires1, mdu_ires2;

int ok, fail, i;
uint8_t r, shifts;

void
test_32div16(char verbose)
{
  ok= fail= 0;
  for (i= 0; i<100; i++)
    {
      lop1= labs(rand()) * abs(rand());
      do {
	iop2= abs(rand()) * abs(rand()%3);
      }
      while (!iop2);

      lres= lop1 / iop2;
      ires1= lop1 % iop2;

      if (verbose)
	printf("%8lx/%4x %10lu/%5u=%10lu,%5u ", lop1, iop2, lop1, iop2, lres, ires1);
      r= mdu_32udiv16(lop1, iop2, &mdu_lres, &mdu_ires1);
      if (verbose)
	printf("mdu=%10lu,%5u ", mdu_lres, mdu_ires1);
      if ((lres != mdu_lres) ||
	  (ires1 != mdu_ires1))
	{
	  if (verbose)
	    printf("fail ");
	  fail++;
	}
      else
	{
	  if (verbose)
	    printf("ok ");
	  ok++;
	}
      if (r &&
	  verbose)
	{
	  if (r&0x80)
	    printf("err ");
	  if (r&0x40)
	    printf("ovr ");
	}
      if (verbose)
	printf("\n");
    }
  printf("32div16 test: succ=%d fails=%d\n\n", ok, fail);
}

void
test_16div16(char verbose)
{
  ok= fail= 0;
  for (i= 0; i<100; i++)
    {
      iop1= abs(rand()) * 1+abs(rand()%2);
      do {
	iop2= abs(rand());
      }
      while (!iop2);

      ires1= iop1 / iop2;
      ires2= iop1 % iop2;

      if (verbose)
	printf("%4x/%4x %5u/%5u=%5u,%5u ", iop1, iop2, iop1, iop2, ires1, ires2);
      r= mdu_16udiv16(iop1, iop2, &mdu_ires1, &mdu_ires2);
      if (verbose)
	printf("mdu=%5u,%5u ", mdu_ires1, mdu_ires2);
      if ((ires1 != mdu_ires1) ||
	  (ires2 != mdu_ires2))
	{
	  if (verbose)
	    printf("fail ");
	  fail++;
	}
      else
	{
	  if (verbose)
	    printf("ok ");
	  ok++;
	}
      if (r &&
	  verbose)
	{
	  if (r&0x80)
	    printf("err ");
	  if (r&0x40)
	    printf("ovr ");
	}
      if (verbose)
	printf("\n");
    }
  printf("16div16 test: succ=%d fails=%d\n\n", ok, fail);
}

void
test_16mul16(char verbose)
{
  ok= fail= 0;
  for (i= 0; i<100; i++)
    {
      iop1= abs(rand()) /*+abs(rand())*/;
      iop2= abs(rand()) /*+abs(rand())*/;

      lres= (unsigned long)iop1 * (unsigned long)iop2;

      if (verbose)
	printf("%4x*%4x %5u*%5u=%10lu ", iop1, iop2, iop1, iop2, lres);
      r= mdu_16umul16(iop1, iop2, &mdu_lres);
      if (verbose)
	printf("mdu=%10lu ", mdu_lres);
      if (lres != mdu_lres)
	{
	  if (verbose)
	    printf("fail ");
	  fail++;
	}
      else
	{
	  if (verbose)
	    printf("ok ");
	  ok++;
	}
      if (r &&
	  verbose)
	{
	  if (r&0x80)
	    printf("err ");
	}
      if (verbose)
	printf("\n");
    }
  printf("16mul16 test: succ=%d fails=%d\n\n", ok, fail);
}

void
test_norm(char verbose)
{
  uint8_t mdu_shifts;
  
  ok= fail= 0;
  for (i= 0; i<100; i++)
    {
      do
	{
	  lop1= labs(rand()) * abs(rand());
	}
      while (lop1 == 0);

      if (lop1 & 0x80000000)
	{
	  lres= lop1;
	  shifts= 0;
	}
      else
	{
	  shifts= 0;
	  lres= lop1;
	  while ((lres & 0x80000000) == 0)
	    {
	      lres<<= 1;
	      shifts++;
	    }
	}
      
      if (verbose)
	printf("%8lx< %10lu=%8lx,%2d ", lop1, lop1, lres, shifts);
      r= mdu_norm(lop1, &mdu_lres, &mdu_shifts);
      if (verbose)
	printf("mdu=%8lx,%2d ", mdu_lres, mdu_shifts);
      if ((lres != mdu_lres) ||
	  (shifts != mdu_shifts))
	{
	  if (verbose)
	    printf("fail ");
	  fail++;
	}
      else
	{
	  if (verbose)
	    printf("ok ");
	  ok++;
	}
      if (r &&
	  verbose)
	{
	  if (r&0x80)
	    printf("err ");
	  if (r&0x40)
	    printf("ovr ");
	}
      if (verbose)
	printf("\n");
    }
  printf("norm test: succ=%d fails=%d\n\n", ok, fail);
}

void
test_shift(char verbose)
{
  ok= fail= 0;
  for (i= 0; i<100; i++)
    {
      do
	{
	  lop1= labs(rand()) * abs(rand());
	}
      while (lop1 == 0);
      do {
	shifts= rand() & 0x1f;
      }
      while (shifts==0);
      
      r= rand() & 1;

      if (r)
	lres= lop1 << shifts;
      else
	lres= lop1 >> shifts;
      
      if (verbose)
	printf("%8lx%c%2d=%8lx ", lop1, r?'<':'>', shifts, lres);
      r= mdu_lshift(lop1, shifts, r, &mdu_lres);
      if (verbose)
	printf("mdu=%8lx ", mdu_lres);
      if (lres != mdu_lres)
	{
	  if (verbose)
	    printf("fail ");
	  fail++;
	}
      else
	{
	  if (verbose)
	    printf("ok ");
	  ok++;
	}
      if (r &&
	  verbose)
	{
	  if (r&0x80)
	    printf("err ");
	}
      if (verbose)
	printf("\n");
    }
  printf("shift test: succ=%d fails=%d\n\n", ok, fail);
}

void main(void)
{
  simif= (__xdata char *)0xffff;
  serial_init(9600);

  test_32div16(0);
  test_16div16(0);
  test_16mul16(0);
  test_norm(0);
  test_shift(0);
  
  *simif= 's';
  while (1)
    {
    }
}
