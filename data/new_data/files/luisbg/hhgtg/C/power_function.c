#include <stdio.h>
#include <stdlib.h>


double power (int m, int n);

/* test power function */
int
main ()
{
  int i;
  for (i = -5; i < 10; ++i)
    printf ("%d: 2^x= %.2f   -3^x= %.2f\n", i, power (2, i), power (-3, i));

  return 0;
}

/* power: raise base to n-th power; n >= 0 || n < 0 */
// since n is passed by value we can use it without changing the original
double
power (int base, int n)
{
  double p = 1;
  int i;

  for (i = 0; i < abs(n); i++)
    p *= base;

  if (n < 0)
    return 1 / p;

  return p;
}
