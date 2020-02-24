/* The prime factors of 13195 are 5, 7, 13 and 29. */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

bool
prime_list (int n, bool ** primes)
{
  int c, d;                     // counters
  if (n < 2)
    return false;

  if (!primes)
    *primes = (bool *) malloc (n * sizeof (bool));
  else
    *primes = (bool *) realloc (*primes, n * sizeof (bool));

  int sqr = (int) sqrt (n);     // square root of n

  for (c = 0; c < n; c++)       // array of primes n might be divisible by
    (*primes)[c] = true;

  for (c = 4; c < sqr; c += 2) {        // set all odd numbers past 2 as non-prime
    (*primes)[c] = false;
  }

  for (c = 2; c <= sqr; c++) {
    if ((*primes)[c]) {
      if (n % c == 0)
        return false;
      else {
        for (d = 2; (d * c) < sqr; d++) {
          (*primes)[d * c] = false;
        }
      }
    }
  }

  return true;
}

int
next_prime (int p, bool * primes)
{
  int i = p + 1;
  while (!(primes[i])) {
    i++;
  }

  return i;
}

void
prime_factors (int num, bool * primes)
{
  int c;

  c = 2;
  while (num > 1) {
    c = next_prime (c, primes);
    if (num % c == 0) {
      num = num / c;
      printf ("%d, ", c);
    }
  }
  printf ("\n");
}


int
main ()
{
  int num = 13195;
  bool *primes = NULL;
  prime_list (num, &primes);
  printf ("prime factors of %d: ", num);
  prime_factors (num, primes);

  num = 111475139;
  prime_list (num, &primes);
  printf ("prime factors of %d: ", num);
  prime_factors (num, primes);

  return 0;
}
