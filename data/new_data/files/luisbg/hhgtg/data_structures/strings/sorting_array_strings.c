/* Sort an array of strings.
 * Example usage of qsort and char **
 *
 * ./sorting_array_strings < example_strings
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 1000
#define MMAX 10000


int compare (const void *i, const void *j)
{
  return strcmp (*(char **)i, *(char **)j);
}


int main ()
{
  int i, n;
  int m = 0;

  char buf[MMAX];

  //char *a[Nmax];
  char **a = malloc (NMAX * sizeof (char *));    // array of pointers

  for (n = 0; n < NMAX; n++) {
    a[n] = &buf[m];
    if (scanf ("%s", a[n]) == EOF)
      break;

    m += strlen (a[n]) + 1;
  }

  qsort (a, n, sizeof (char *), compare);
  for (i = 0; i < n; i++)
    printf ("%s\n", a[i]);

  return 0;
}
