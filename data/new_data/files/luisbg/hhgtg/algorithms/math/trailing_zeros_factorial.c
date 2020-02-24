#include <stdio.h>


unsigned long
factorial (unsigned long n)
{
  if (n == 1)
    return 1;

  return n * factorial (n - 1);
}

int
trailing_zeros (unsigned long n)
{
  int c = 0;
  unsigned long tmp;

  for (tmp = n; tmp > 10; tmp = tmp / 10) {
    if ((tmp % 10) == 0)
      c++;
    else
      break;
  }

  return c;
}

int
powers_of_five (int n)
{
  int c = 0;
  int i;
  for (i = 5; n / i > 0; i *= 5) {
    c++;
  }

  return c;
}

int
direct_trailing_zeros (int n)
{
  int z = 0;

  for (; n > 4; n--) {
    if (n % 5 == 0)
      z += powers_of_five (n);
  }

  return z;
}


int
main ()
{
  unsigned int n = 26;
  // unsigned long f;
  // int z;

  // f = factorial ((unsigned long) n);
  // z = trailing_zeros (f);
  // printf ("n: %d; f: %d; z: %d\n", n, f, z);

  printf ("n: %d; z: %d\n", n, direct_trailing_zeros (n));

  return 0;
}
