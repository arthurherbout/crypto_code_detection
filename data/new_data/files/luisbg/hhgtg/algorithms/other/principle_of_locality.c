/* https://en.wikipedia.org/wiki/Locality_of_reference */
/* compile with gcc -O1 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>


void
slow_matrix_multiplication (int a[500][500], int b[500][500])
{
  int c[500][500];
  int res = 0;

  // Approach: loop over i, j and k
  // b[k][j] isn't in the cache
  for (int i = 0; i < 500; i++)
    for (int j = 0; j < 500; j++)
      for (int k = 0; k < 500; k++)
        c[i][j] = a[i][k] * b[k][j];

  for (int m = 0; m < 500; m++) {
    for (int n = 0; n < 500; n++)
      res += c[m][n];
  }

  printf ("res: %d\nslow ", res);
}

void
fast_matrix_multiplication (int a[500][500], int b[500][500])
{
  int c[500][500];
  int res = 0;

  // Approach: switch loop order of k and j
  // the reads of b[k][j] are in the cache
  for (int i = 0; i < 500; i++)
    for (int k = 0; k < 500; k++)
      for (int j = 0; j < 500; j++)
        c[i][j] = a[i][k] * b[k][j];

  for (int m = 0; m < 500; m++) {
    for (int n = 0; n < 500; n++)
      res += c[m][n];
  }

  printf ("res: %d\nfast ", res);
}

void
init_matrix (int x[500][500])
{
  for (int m = 0; m < 500; m++) {
    for (int n = 0; n < 500; n++)
      x[m][n] = rand () % 100;
  }
}

int
main ()
{
  struct timespec start, end;
  uint64_t delta_us;
  int a[500][500], b[500][500];

  printf ("principle of locality\n");

  srand (time (NULL));
  init_matrix (a);
  init_matrix (b);

  clock_gettime (CLOCK_MONOTONIC_RAW, &start);
  slow_matrix_multiplication (a, b);
  clock_gettime (CLOCK_MONOTONIC_RAW, &end);
  delta_us =
      (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec -
      start.tv_nsec) / 1000;
  printf ("time delta: %ld nanoseconds\n\n", delta_us);

  clock_gettime (CLOCK_MONOTONIC_RAW, &start);
  fast_matrix_multiplication (a, b);
  clock_gettime (CLOCK_MONOTONIC_RAW, &end);
  delta_us =
      (end.tv_sec - start.tv_sec) * 1000000 + (end.tv_nsec -
      start.tv_nsec) / 1000;
  printf ("time delta: %ld nanoseconds\n", delta_us);

  return 0;
}
