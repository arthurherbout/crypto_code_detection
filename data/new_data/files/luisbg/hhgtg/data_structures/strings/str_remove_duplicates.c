#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


int *
remove_dup (int A[], int n)
{
  int i, nr = 0;
  int *intcount = (int *) calloc (9, (sizeof (int)));
  int *tmp = (int *) calloc (n, (sizeof (int)));
  int *output = NULL;

  // count the occurrences of each char
  for (i = 0; i < n; i++) {
    intcount[A[i]]++;
  }

  // find the first occurence in the string with one ocurrence
  for (i = 0; i < n; i++)
    if (intcount[A[i]] > 1) {
      tmp[nr] = A[i];
      nr++;

      // reset for next ocurrences
      intcount[A[i]] = 0;
    }
  // create properly sized output array
  if (nr > 0) {
    output = (int *) malloc (nr * sizeof (int));
    for (i = 0; i < nr; i++)
      output[i] = tmp[i];
  }

  free (intcount);
  free (tmp);

  return output;
}

void
print_int_arr (int A[], int n)
{
  int i;

  if (n > 0) {
    for (i = 0; i < n; i++) {
      printf ("%d ", A[i]);
    }
  } else {
    printf ("empty");
  }

  printf ("\n");
}

int
main ()
{
  int a[10] = { 1, 2, 3, 4, 4, 3, 2, 5, 6, 5 };
  int b[5] = { 9, 9, 1, 1, 2 };
  int c[3] = { 1, 1, 1 };
  int d[1] = { 3 };

  int *o;

  printf ("a: ");
  print_int_arr (a, 10);
  o = remove_dup (a, 10);
  print_int_arr (o, 4);

  printf ("\nb: ");
  print_int_arr (b, 5);
  o = remove_dup (b, 5);
  print_int_arr (o, 2);

  printf ("\nc: ");
  print_int_arr (c, 3);
  o = remove_dup (c, 3);
  print_int_arr (o, 1);

  printf ("\nd: ");
  print_int_arr (d, 1);
  o = remove_dup (d, 1);
  print_int_arr (o, 0);

  return 0;
}
