/* Compute the sign of an integer */

#include <stdio.h>

#define CHAR_BIT 8        // the number of bits per byte (normally 8)

int
sign_direct (int v)
{
  return -(v < 0);        // if v < 0 then -1, else 0
}

int
sign_bitwise (int v)
{                         // avoid branching on CPUs with flag registers (IA32):
  return v >> (sizeof (int) * CHAR_BIT - 1);
}

int
opposite_sign_numbers (int x, int y)
{
  return (x ^ y) < 0;           // true iff x and y have opposite signs
}

int
main ()
{
  int nums[8] = { 1, -1, 8, -8, 16, -16, 100, -100 };
  int i;

  for (i = 0; i < 8; i++) {
    printf ("direct:  sign of %d: %d\n", nums[i], sign_direct (nums[i]));
  }
  printf ("\n");
  for (i = 0; i < 8; i++) {
    printf ("bitwise: sign of %d: %d\n", nums[i], sign_bitwise (nums[i]));
  }

  for (i = 1; i < 8; i++) {
    printf ("do %d and %d have opposite signs?: %d\n", nums[0], nums[i],
        opposite_sign_numbers (nums[0], nums[i]));
  }
  return 0;
}
