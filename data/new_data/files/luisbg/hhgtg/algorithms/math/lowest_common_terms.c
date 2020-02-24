#include <stdlib.h>
#include <stdio.h>


typedef struct fraction {
  int num;
  int den;
} fraction;


int biggest_common_divisor (int num, int den)
{
  int i;
  for (i = num; i > 0; i--) {
    if (num % i == 0 && den % i == 0)
        return i;
  }

  return -1;
}

fraction lowest_common_terms (int num, int den)
{
  fraction lwt;
  int bcd = 0;

  if (den % num == 0) {
    lwt.num = 1;
    lwt.den = den / num;
  } else {
    bcd = biggest_common_divisor (num, den);
    if (bcd > 1) {
      lwt.num = num / bcd;
      lwt.den = den / bcd;
    } else {
      lwt.num = num;
      lwt.den = den;
    }
  }

  return lwt;
}

int curious_fraction(int num, int den)
{
  int i, j, p, curious_num = -1, curious_den = -1;
  int digits_of_num[2] = {0, 0};
  int digits_of_den[2] = {0, 0};
  float fraction, check;

  if (num == den || num >= den)
    return 0;

  // get digits from numerator
  i = num;
  p = 0;
  while (i > 0) {
    digits_of_num[p++] = i % 10;
    i = i / 10;
  }

  // get digits from denominator
  i = den;
  p = 0;
  while (i > 0) {
    digits_of_den[p++] = i % 10;
    i = i / 10;
  }

  // look for non trivial match
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      if (digits_of_num[i] != 0 && 
          i != j &&
          digits_of_num[i] == digits_of_den[j]) {
        curious_num = (i == 0)? 1: 0;
        curious_den = (j == 0)? 1: 0;
      }
    }
  }

  // no match
  if (curious_num == -1)
    return 0;

  // we had a match, let's check if fraction and curious fraction are equal
  fraction =  (float) num / den;
  check = (float) digits_of_num[curious_num] / digits_of_den[curious_den];
  return (fraction == check);
}

int main ()
{
  int i, j;
  fraction frct;

  for (i = 1; i < 100; i++) {
    for (j = 1; j < 100; j++) {
      if (curious_fraction (i, j)) {
        frct = lowest_common_terms (i, j);
        printf ("curious fraction: %d / %d -> %d / %d\n", i, j,
                frct.num, frct.den);
      }
    }
  }

  return 0;
}
