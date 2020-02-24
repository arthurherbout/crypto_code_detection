/* Write a function that determines the number of 1 bits in the computer’s
 * internal representation of a given integer.
 */

#include <stdio.h>


int
count_ones (int n)
{
  int ones = 0;

  while (n != 0) {
    if ((n & 1) == 1)
      ones++;
    n = n >> 1;
  }

  return ones;
}

int
count_ones_fast (int n)
{
  int ones = 0;

  while (n != 0) {
    n = n & (n - 1);
    ones++;
  }

  return ones;
}

int
main ()
{
  int i = 55;

  printf ("number of 1's in bit representation of %d: %d\n", i, count_ones (i));

  i = 57;
  printf ("number of 1's in bit representation of %d: %d\n", i,
      count_ones_fast (i));

  return 0;
}
