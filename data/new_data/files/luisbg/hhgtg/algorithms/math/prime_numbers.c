#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool
is_it_prime (int n)
{
  if (n < 2)
    return false;

  for (int c = 2; c * c <= n; c++) {
    if (n % c == 0)
      return false;
  }

  return true;
}

// Eratosthenes works to check small numbers. For big numbers there could be
// memory issues.
// Specially suited when a cache needs to be generated to check if a series of
// numbers are prime.
void
generate_eratosthenes (int max, bool * primes)
{
  primes[0] = false;
  primes[1] = false;
  for (int c = 2; c < max; c++) // init all to prime
    primes[c] = true;

  for (int c = 2; c * c < max; c++) {   // run through primes and multiples of them.
    if (primes[c]) {            // mark the multiples as not primes.
      for (int d = c + c; d < max; d += c) {
        primes[d] = false;
      }
    }
  }

}

void
generate_primes_window (int start, int end, bool * primes)
{
  if (start >= end)
    return;

  if (start < 3) {
    printf ("Use regular eratosthenes if you want to start from 1 or 2\n");
    free (primes);
    return;
  }

  for (int c = 0; c < (end - start); c++) {
    primes[c] = true;
  }

  for (int c = 2; (c * c) <= end; c++) {
    for (int d = start; d < end; d++) {
      if (primes[d - start] && (d % c == 0)) {
        while (d < end) {
          primes[d - start] = false;
          d += c;
        }

        break;
      }
    }
  }
}

bool
is_it_prime_eratosthenes (int n, bool * primes)
{
  return primes[n];
}

int
main ()
{
  int n;
  int start = 100, end = 200;
  int max = 1000;               // speed test with 1 thousand
  int found = 0;

  bool *primes = (bool *) malloc (max * 8 * sizeof (bool));
  generate_eratosthenes (max * 8, primes);

  printf ("list %d first primes:\n", max);
  for (n = 1; found < max; n++) {
    if (is_it_prime_eratosthenes (n, primes)) {
      printf ("%d ", n);
      found++;
    }
  }
  printf ("\n");

  bool *window = (bool *) malloc ((end - start) * sizeof (bool));
  generate_primes_window (start, end, window);

  printf ("The window %d to %d has the following primes:\n", start, end);
  for (int i = start; i < end; i++) {
    if (window[i - start])
      printf ("%d ", i);
  }
  printf ("\n");

  free (primes);
  return 0;
}
