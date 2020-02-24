// Print all powers of 2 up to N

#include <stdio.h>

int
powers_of_2 (int n)
{
  if (n <= 0)
    return -1;

  if (n == 1) {
    printf (" 1");
    return 1;
  }

  int prev = powers_of_2 (n / 2);
  int curr = prev * 2;
  printf (" %d", curr);

  return curr;
}

int
main ()
{
  int set[6] = { 4, 7, 17, 32, 1000, 5000 };

  for (int c = 0; c < 6; c++) {
    printf ("All powers of 2 up to %d:", set[c]);
    powers_of_2 (set[c]);
    printf ("\n");
  }

  return 0;
}
