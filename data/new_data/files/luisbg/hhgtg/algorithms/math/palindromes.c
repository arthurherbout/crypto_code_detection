/* A palindromic number reads the same both ways. The largest palindrome made */
/* from the product of two 2-digit numbers is 9009 = 91 × 99. */
/* Find the largest palindrome made from the product of two 3-digit numbers. */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>


bool
is_palindrome (int num)
{
  int num_digits;
  int tmp = num;
  int i;

  for (num_digits = 0; tmp > 0; num_digits++) {
    tmp = tmp / 10;
  }
  int *digits = (int *) malloc (num_digits * sizeof (int));

  tmp = num;
  for (i = 0; i < num_digits; i++) {
    digits[i] = tmp % 10;
    tmp = tmp / 10;
  }

  tmp = num_digits - 1;
  i = 0;
  while (i < tmp) {
    if (digits[i] != digits[tmp]) {
      free (digits);
      return false;
    }

    i++;
    tmp--;
  }

  free (digits);

  return true;
}

unsigned long
biggest_palindrome (int top)
{
  int c, d;
  unsigned long biggest = 0;
  unsigned long prod;

  for (d = top; d > (sqrt (top) * 2); d--) {
    for (c = top; c > (sqrt (top) * 2); c--) {
      prod = c * d;
      if (is_palindrome (prod))
        break;
    }

    if (prod > biggest) {
      biggest = prod;
      // printf ("%d, %d\n", d, c);
    }
  }


  return biggest;
}

int
main ()
{
  printf ("is %d a palindrome? %s\n", 1234,
      is_palindrome (1234) ? "yes" : "no");
  printf ("is %d a palindrome? %s\n", 1331,
      is_palindrome (1331) ? "yes" : "no");
  printf ("is %d a palindrome? %s\n", 1, is_palindrome (1) ? "yes" : "no");

  printf ("largest palindrome of two digit numbers: %lu\n",
      biggest_palindrome (999));

  return 0;
}
