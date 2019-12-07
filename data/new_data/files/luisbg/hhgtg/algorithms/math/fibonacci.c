#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


int
fibonacci_recursive (int i)     // O(n^2)
{
  if (i < 0)
    return -1;

  if (i > 92)
    return -1;

  if (i == 0 || i == 1)
    return i;

  return fibonacci_recursive (i - 1) + fibonacci_recursive (i - 2);
}

uint64_t
fibonacci_dynamic (int i, uint64_t fib[])       // O(n)
{
  if (i < 0)
    return -1;

  if (i > 92)
    return -1;

  if (i == 0 || i == 1)
    return i;

  // return cache
  if (fib[i] != 0)
    return fib[i];

  // cache result
  fib[i] = fibonacci_dynamic (i - 1, fib) + fibonacci_dynamic (i - 2, fib);
  return fib[i];
}

uint64_t
fibonacci_iterative (int i)
{
  if (i < 0)
    return -1;

  if (i > 92)
    return -1;

  if (i == 0)
    return 0;

  uint64_t a = 0, b = 1, tmp;

  for (int c = 1; c < i; c++) {
    tmp = a;
    a = b;
    b = tmp + b;
  }

  return b;
}

int
main ()
{
  int seq[10] = { 1, 2, 3, 4, 5, 10, 25, 50, 75, 90 };

  int c;
  for (c = 0; c < 10; c++) {
    uint64_t *fib = (uint64_t *) calloc (seq[c] + 1,
        sizeof (uint64_t));
    printf ("fibonnaci number %d: %ld\n", seq[c],
        fibonacci_dynamic (seq[c], fib));
    free (fib);

    // printf("%lld\n", fibonacci_iterative (seq[c]));
  }
}
