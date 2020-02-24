#include <stdlib.h>
#include <stdio.h>


void find_kth_element_r (int *l, int low, int high, int k);
static int partition (int *l, int low, int high);
static int get_middle_pivot (int low, int high);
static void swap (int *a, int *b);
static void print_list (int *l, int len);


/* Recursive find kth element */
void find_kth_element_r (int *l, int low, int high, int k)
{
  int i;

  if (high <= low)
    return;

  i = partition (l, low, high);

  if (i > k)
    find_kth_element_r (l, low, i - 1, k);
  if (i < k)
    find_kth_element_r (l, i + 1, high, k);
}

/* Non-recursive find kth element */
void find_kth_element_nr (int *l, int low, int high, int k)
{
  int i;

  while (high > low) {
    i = partition (l, low, high);
    if (i >= k)
      high = i - 1;
    if (i <= k)
      low = i + 1;
  }
}

static int
get_middle_pivot (int low, int high)
{
  return (low + high) / 2;
}

static void
swap (int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

static void
print_list (int *l, int len)
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
  int c;
  int middle;

  int k = get_middle_pivot (low, high);
  int pivot = l[k];
  swap (&l[low], &l[k]);        // move pivot out of array to order

  c = low + 1;
  middle = high;
  while (c <= middle)           // run the array
  {
    while ((c <= high) && (l[c] <= pivot))      // before pivot
      c++;
    while ((middle >= low) && (l[middle] > pivot))      // after pivot
      middle--;
    if (c < middle)
      swap (&l[c], &l[middle]);
  }

  swap (&l[low], &l[middle]);   // bring the pivot to its place

  return middle;
}


int
main ()
{
  int l[30];
  int size = 30;
  int k = 15;
  int c;

  for (c = 0; c < size; c++)
    l[c] = rand () % 100;

  printf ("first list (solve recursively):\n");
  print_list (l, size - 1);

  find_kth_element_r (l, 0, size - 1, k);
  printf ("\nelement in position %d is: %d\n", k, l[k]);

  for (c = 0; c < size; c++)
    l[c] = rand () % 100;

  printf ("second list (solve non-recursively):\n");
  print_list (l, size - 1);

  find_kth_element_nr (l, 0, size - 1, k);
  printf ("\nelement in position %d is: %d\n", k, l[k]);

  return 0;
}
