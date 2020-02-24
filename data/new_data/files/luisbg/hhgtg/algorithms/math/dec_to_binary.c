/* Write a function to compute the binary representation of an integer. Return
 * the representation as a number. Assume the input will be a positive integer
 * only.
 *
 * Examples:
 * computeBinary(6) ==> 110
 * computeBinary(8) ==> 1000
 */

#include <stdio.h>


int
dec_to_binary (int num)
{
  int bin = 0;
  int digit = 1;

  while (num > 0) {
    bin += (num % 2) * digit;
    num = num / 2;
    digit = digit * 10;
  }

  return bin;
}

int
to_binary_recursive (int num)
{
  if (num <= 0)
    return 0;
  else
    /* Recursively keep calculating the remainder
       and then move to the remaining part */
    return (num % 2) + (10 * to_binary_recursive (num / 2));
}


int
main ()
{
  int i;
  for (i = 0; i < 16; i++)
    printf ("%2d :: \t%6d\n", i, dec_to_binary (i));
  for (; i < 32; i++)
    printf ("%2d :: \t%6d\n", i, to_binary_recursive (i));

  return 0;
}
