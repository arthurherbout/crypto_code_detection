#include <stdio.h>


// swap assuming 32 bit integers
int
swap_odd_even_bits (int x)
{
  int oddbits = (x & 0xAA) >> 1;
  int evenbits = (x & 0x55) << 1;

  return oddbits | evenbits;
}

int
main ()
{
  int numbers[5] = { 14, 31, 0, 27, 5 };
  int i;

  for (i = 0; i < 5; i++)
    printf ("%d -> %d\n", numbers[i], swap_odd_even_bits (numbers[i]));

  return 0;
}
