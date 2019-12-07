#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t
factorial (int n)
{
  if (n < 0 || n > 20)
    return -1;

  if (n == 0)
    return 1;

  return n * factorial (n - 1);
}

uint64_t
do_all_factorials (int n, uint64_t *results, int level)
{
  if (n > 1)                    // recursive case
    results[level] = n * do_all_factorials (n - 1, results, level + 1);
  else
    results[level] = 1;

  return results[level];
}

uint64_t *
all_factorials (int n)
{
  if (n < 0 || n > 20)
    return NULL;

  uint64_t *results = (uint64_t *) malloc (n *
      sizeof (uint64_t));
  do_all_factorials (n, results, 0);

  return results;
}

uint64_t
factorial_iterative (int n)
{
  uint64_t ret = 1;

  for (int c = n; c > 1; c--)       // 0 or 1 fall through
    ret *= c;

  return ret;
}

int
main ()
{
  int set[10] = { 1, 2, 3, 5, 8, 10, 12, 15, 18, 20 };

  for (int c = 0; c < 10; c++) {
    if (c % 2 == 0)
      printf ("factorial of %d: %ld\n", set[c], factorial (set[c]));
    else
      printf ("factorial of %d: %ld\n", set[c], factorial_iterative (set[c]));
  }

  int n = 20;
  int lvl;
  uint64_t *results = (uint64_t *) malloc (n *
      sizeof (uint64_t));
  results = all_factorials (n);
  for (int c = 0; c < 20; c++) {
    lvl = (n - c) % n;
    if (!lvl)
      lvl = 20;
    printf ("%d: %ld\n", lvl, results[c]);
  }
}
