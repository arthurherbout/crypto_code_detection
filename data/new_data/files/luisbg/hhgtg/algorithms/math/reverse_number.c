/* Reverse the digits of a number
 *
 * 234 -> 432
 * -1017 -> -7101
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


int
reverse (int x)
{
  int res = 0;
  int neg = 0;

  if (x < 0) {
    x *= -1;
    neg = 1;
  }

  while (x > 0) {
    res = (x % 10) + (res * 10);
    x = x / 10;
  }

  if (neg)
    res *= -1;

  return res;
}

int
main ()
{
  int i, c;
  srand (time (NULL));

  for (c = 0; c < 10; c++) {
    i = rand () % 100000;
    if (rand () < RAND_MAX / 2)
      i *= -1;

    printf ("%6d  \t->  \t%6d\n", i, reverse (i));
  }

  return 0;
}
