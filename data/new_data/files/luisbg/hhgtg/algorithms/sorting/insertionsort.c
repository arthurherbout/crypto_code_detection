#include <stdlib.h>
#include <stdio.h>

void insertionsort (int *ar, int ar_size);

static void
print_array (int *l, int len)
{
  int c;
  for (c = 0; c < len; c++) {
    printf ("%d ", l[c]);
  }
  printf ("\n");
}


void
insertion_sort (int *ar, int ar_size)
{
  int c, j, tmp;

  for (c = ar_size - 1; c > 0; c--)
    if (ar[c-1] > ar[c]) {
      tmp = ar[c-1];
      ar[c-1] = ar[c];
      ar[c] = tmp;
    }

  for (c = 2; c < ar_size; c++) {
    j = c;
    tmp = ar[c];

    while (tmp < ar[j - 1]) {
      ar[j] = ar[j - 1];

      j--;
    }

    ar[j] = tmp;
    // print_array (ar, ar_size);
  }

}


int
main ()
{
  int ar[50];
  int size = 50;
  int c;
  for (c = 0; c < size; c++)
    ar[c] = rand () % 100;

  printf ("out of order:\n");
  print_array (ar, size);

  printf ("in order:\n");
  insertion_sort (ar, size);
  print_array (ar, size);

  return 0;
}
