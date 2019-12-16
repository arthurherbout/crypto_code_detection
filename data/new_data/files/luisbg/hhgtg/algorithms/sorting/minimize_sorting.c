#include <stdio.h>
#include <stdlib.h>


int
shrink_left (int *arr, int min_index, int end_left)
{
  int c;
  for (c = end_left - 1; c >= 0; c--) {
    if (arr[c] <= arr[min_index])
      return c + 1;
  }

  return 0;
}

int
shrink_right (int *arr, int max_index, int start_right, int len)
{
  int c;
  for (c = start_right; c < len; c++) {
    if (arr[c] >= arr[max_index])
      return start_right;
  }

  return len - 1;
}

int
find_end_of_left_subsequence (int *arr, int len)
{
  int c;
  for (c = 1; c < len; c++) {
    if (arr[c] < arr[c - 1])
      return c - 1;
  }

  return len - 1;
}

int
find_start_of_right_subsequence (int *arr, int len)
{
  int c;
  for (c = len - 1; c >= 0; c--) {
    if (arr[c] < arr[c - 1])
      return c + 1;
  }

  return 0;
}

int *
minimize_direct (int *arr, int len)
{
  int *res = (int *) malloc (2 * sizeof (int));

  /* find left subsequence */
  int end_left = find_end_of_left_subsequence (arr, len);

  /* find right subsequence */
  int start_right = find_start_of_right_subsequence (arr, len);

  /* find min and max element of middle */
  int min_index = end_left + 1;
  if (min_index >= len)
    return NULL;                     // already sorted

  int max_index = start_right - 1;
  int c;
  for (c = end_left; c <= start_right; c++) {
    if (arr[c] < arr[min_index])
      min_index = c;
    if (arr[c] > arr[max_index])
      max_index = c;
  }

  /* slide left until less than array [min_index] */
  res[0] = shrink_left (arr, min_index, end_left);

  /* slide right until greater than array [max_index] */
  res[1] = shrink_right (arr, max_index, start_right, len);

  return res;
}

void
swap (int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

int
get_pivot (int low, int high)
{
  return (low + high) / 2;
}

void
quicksort (int *l, int low, int high)
{
  if (low >= high)
    return;

  int c;
  int middle;

  int k = get_pivot (low, high);
  int pivot = l[k];
  swap (&l[low], &l[k]);

  c = low + 1;
  middle = high;
  while (c <= middle) {
    while ((c <= high) && (l[c] <= pivot))
      c++;
    while ((middle >= low) && (l[middle] > pivot))
      middle--;
    if (c < middle)
      swap (&l[c], &l[middle]);
  }

  swap (&l[low], &l[middle]);

  quicksort (l, low, middle - 1);
  quicksort (l, middle + 1, high);
}

int *
minimize_quicksort (int *arr, int len)
{
  int *tmp = (int *) malloc (len * sizeof (int));
  int *res = (int *) malloc (2 * sizeof (int));
  int c;

  for (c = 0; c < len; c++)
    tmp[c] = arr[c];

  quicksort (tmp, 0, len - 1);

  for (c = 0; c < len && (tmp[c] == arr[c]); c++) {;
  }
  res[0] = c;

  for (c = len - 1; c >= 0 && (tmp[c] == arr[c]); c--) {;
  }
  res[1] = c;

  return res;
}


int
main ()
{
  int i;
  int len = 13;
  int input[13] = { 1, 2, 4, 7, 10, 11, 7, 12, 6, 7, 16, 18, 19 };
  int *output;

  printf ("unsorted:");
  for (i = 0; i < len; i++)
    printf (" %d", input[i]);
  printf ("\n");

  output = minimize_direct (input, len);

  printf ("first unsorted: %d\nlast unsorted: %d\n", output[0], output[1]);

  return 0;
}
