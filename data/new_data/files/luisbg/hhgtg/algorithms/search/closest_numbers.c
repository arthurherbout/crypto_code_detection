#include <stdio.h>
#include <stdlib.h>


void find_smallest_diff (int ar_size, int *ar);
static void print_list (int len, int *l);
static int partition (int *l, int low, int high);
static void quicksort (int *l, int low, int high);


static void
print_list (int len, int *l)
{
  int c;
  for (c = 0; c < len; c++) {
    printf ("%d ", l[c]);
  }
  printf ("\n");
}

static int
partition (int *l, int low, int high)
{
  int c, tmp, pivot, middle;

  int *low_ar = (int *) malloc (high * sizeof (int));
  int *high_ar = (int *) malloc (high * sizeof (int));
  int size_low = 0, size_high = 0;

  pivot = l[low];
  for (c = low + 1; c <= high; c++) {
    if (l[c] <= pivot) {
      low_ar[size_low] = l[c];
      size_low++;
    } else {
      high_ar[size_high] = l[c];
      size_high++;
    }
  }

  c = low;
  for (tmp = 0; tmp < size_low; tmp++) {
    l[c] = low_ar[tmp];
    c++;
  }
  l[c] = pivot;
  middle = c;
  c++;
  for (tmp = 0; tmp < size_high; tmp++) {
    l[c] = high_ar[tmp];
    c++;
  }

  free (low_ar);
  free (high_ar);

  return middle;
}

static void
quicksort (int *l, int low, int high)
{
  if (low >= high)
    return;

  int middle;

  middle = partition (l, low, high);
  quicksort (l, low, middle - 1);
  quicksort (l, middle + 1, high);
}

void
find_smallest_diff (int ar_size, int *ar)
{
  int i;
  int tmp_diff;

  quicksort (ar, 0, ar_size - 1);

  int smallest_diff = ar[1] - ar[0];
  if (smallest_diff < 0)
    smallest_diff *= -1;

  /* Find the smallest difference value */
  for (i = 2; i < ar_size; i++) {
    tmp_diff = ar[i] - ar[i - 1];
    if (tmp_diff < 0)
      smallest_diff *= -1;
    if (tmp_diff < smallest_diff) {
      smallest_diff = tmp_diff;
    }
  }

  /* There could be multiple sets with the same smallest difference */
  for (i = 1; i < ar_size; i++) {
    tmp_diff = ar[i] - ar[i - 1];
    if (tmp_diff < 0)
      smallest_diff *= -1;
    if (tmp_diff == smallest_diff) {
      printf ("closest: %d %d ", ar[i - 1], ar[i]);
    }
  }
  printf ("\n");
}


int
main (void)
{

  int ar_size = rand () % 50;
  int ar[ar_size], i;
  for (i = 0; i < ar_size; i++) {
    ar[i] = rand () % 10000;
    if (rand () % 2)
      ar[i] *= -1;
  }

  print_list (ar_size, ar);
  find_smallest_diff (ar_size, ar);

  return 0;
}
