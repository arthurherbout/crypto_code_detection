/* Key-indexed counting
 *
 * Runs in O(N), where N is the range of possible entries of the list
 *
 */

#include <stdlib.h>
#include <stdio.h>

#define RANGE_MAX 100

void key_index_count (int a[], int len);
void print_list (int a[], int len);


/* create an array size of range and count instances of values */
void key_index_count (int a[], int len)
{
  int i, j;
  int *cnt = (int *) calloc (RANGE_MAX, sizeof (int));

  // Count instances of each number inside the range
  for (i = 0; i <= (len - 1); i++)
    cnt[a[i]]++;

  // Run through range and place numbers in ordered list
  for (i = 0, j = 0; i < RANGE_MAX; i++)
    while (cnt[i]--) {
      a[j++] = i;
    }

  return;
}

/* print the list of values */
void
print_list (int a[], int len)
{
  int i;

  for (i = 0; i < len; i++)
    printf ("%3d  ", a[i]);
  printf ("\n \n");

  return;
}


int main (int argc, char *argv[])
{
  int i;
  int N = atoi (argv[1]);

  int *a = malloc (N * sizeof (int));

  printf ("sorting %d random numbers from 0 to %d:\n", N, RANGE_MAX);
  for (i = 0; i < N; i++)
    a[i] = rand () % (RANGE_MAX - 1);

  // Print list, sort and print again
  print_list (a, N);
  key_index_count (a, N);
  print_list (a, N);

  return 0;
}
