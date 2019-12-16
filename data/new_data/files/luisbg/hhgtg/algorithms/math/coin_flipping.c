/* A table that shows the result of simulating M experiments of flipping a coin
 * N times. Displaying an asterik for every 10 occurrences. 
 *
 * Known as the normal approximation, bell-shaped curve, and Bernoulli
 * distribution
 *
 * example:
 * ./coin_flipping 32 1000
 *
 * */

#include <stdlib.h>
#include <stdio.h>

int heads()
{
  return rand() < RAND_MAX / 2;
}

int main (int argc, char *argv[])
{
  int i, j, c;
  int N = atoi(argv[1]);
  int M = atoi(argv[2]);

  int *f = malloc ((N+1) * sizeof (int));

  /* init array to 0 */
  for (j = 0; j <= N; j++)
    f[j] = 0;

  /* run M experiments with N flips */
  for (i = 0; i < M; i++, f[c]++) {
    for (c = 0, j = 0; j <= N; j++) {
      if (heads())
        c++;
    }
  }

  /* print table */
  for (j = 0; j <= N; j++) {
    printf ("%2d ", j);
    for (i = 0; i < f[j]; i += 10)
      printf ("*");
    printf ("\n");
  }

  return 0;
}
