/* Simple recursive example to find the maximum number in a list
 *
 * usage: ./max_by_divide_and_conquer 1 2 7 -1 3 4 5
 * returns 7
 *
 * Not faster than iteratively, but a simple extrapolated example of
 * divide and conquer
 */

#include <stdlib.h>
#include <stdio.h>


/* Recursive function to find the maximum number of the set */
int
max (int a[], int l, int r)
{
  int u, v, m;

  m = (l + r) / 2;

  if (l == r)
    return a[l];

  u = max (a, l, m);
  v = max (a, m + 1, r);

  if (u > v)
    return u;
  else
    return v;
}

/* Wrapper function to have a cleaner interface */
int find_max (int a[], int size)
{
  return max (a, 0, size - 1);
}


int
main (int argc, char *argv[])
{
  int c;
  int fixed_array[12] = {0, 2, 12, 5, 7, 3, 19, 9, 1, 8, 15, 11};
  int *arg_array = NULL;

  if (argc == 1) {
    printf ("max: %d\n", find_max (fixed_array, 12));
  } else {
    arg_array = (int *) malloc ((argc - 1) * sizeof (int));

    for (c = 1; c < argc; c++) {
      arg_array[c - 2] = atoi (argv[c]);
    }

    printf ("max: %d\n", find_max (arg_array, argc - 1));
  }

  return 0;
}
