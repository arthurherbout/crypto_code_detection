#include <stdio.h>
#include <stdlib.h>


/* recursive power: raise base to n-th power; n >= 0 || n < 0 */
double
recursive_power (double b, int n)
{
  if (n == 0)
    return 1.0;

  if (n > 0)
    return (b * recursive_power (b, n - 1));

  if (n < 0)
    return (recursive_power (1 / b, -1 * n));

  return 1.0;
}

/* iterative power */
double
iterative_power (double x, int n)
{
    double power;
    int neg = 0;

    if (n < 0) {
        n = n * -1;
        neg = 1;
    }

    for (power = 1; n > 0; --n)
        power = power * x;

    if (neg)
        power = 1 / power;

    return power;
 }


/* test power function */
int
main ()
{
  int i;
  for (i = -5; i < 10; ++i)
    if (i % 2 == 0)
      printf ("%d: 2^x= %.2f   -3^x= %.2f\n", i, recursive_power (2, i), recursive_power (-3, i));
    else
      printf ("%d: 2^x= %.2f   -3^x= %.2f\n", i, iterative_power (2, i), iterative_power (-3, i));

  return 0;
}
