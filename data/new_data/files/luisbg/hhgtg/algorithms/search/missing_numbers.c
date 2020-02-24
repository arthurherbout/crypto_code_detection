/*  Find the missing numbers from B in A without changing up the order
 *
 *  Run with: ./missing_numbers < missing_numbers_input.txt */

#include <stdio.h>
#include <stdlib.h>


void check_missing (int *list_a, int *list_b, int low, int high);
static int *range_count (int size, int *list, int low, int high);

void
check_missing (int *list_a, int *list_b, int low, int high)
{
  int c;
  int range = high - low;

  // If the count is different print the number
  for (c = 0; c < range; c++) {
    if (list_a[c] != list_b[c])
      printf ("%d ", c + low);
  }

  printf ("\n");
}

int *
range_count (int size, int *list, int low, int high)
{
  int *count_ar = (int *) malloc ((high - low) * sizeof (int));
  int c, value, range;

  range = high - low;
  // Initialize all elements of count array to 0
  for (c = 0; c < range; c++) {
    count_ar[c] = 0;
  }

  // Go through the list incrementing the count of each value seen
  for (c = 0; c < size; c++) {
    value = list[c] - low;
    count_ar[value]++;
  }

  return count_ar;
}


int
main ()
{
  int size_a, size_b;
  int i;

  scanf ("%d", &size_a);
  int *ar_a = (int *) malloc (size_a * sizeof (int));
  for (i = 0; i < size_a; i++) {
    scanf ("%d ", &ar_a[i]);
  }

  scanf ("%d", &size_b);
  int *ar_b = (int *) malloc (size_b * sizeof (int));
  for (i = 0; i < size_b; i++) {
    scanf ("%d ", &ar_b[i]);
  }

  int *a_count = range_count (size_a, ar_a, -10000, 10000);
  int *b_count = range_count (size_b, ar_b, -10000, 10000);

  check_missing (a_count, b_count, -10000, 10000);

  free (ar_a);
  free (ar_b);
  free (a_count);
  free (b_count);

  return 0;
}
