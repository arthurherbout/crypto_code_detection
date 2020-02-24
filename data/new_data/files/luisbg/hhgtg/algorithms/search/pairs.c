/* Given N integers, count the total pairs of integers that have a   */
/* difference                                                        */

#include <stdio.h>

static void
swap (int *a, int *b)
{
  int tmp = *a;
  *a = *b;
  *b = tmp;
}

static int
partition (int *l, int low, int high)
{
  int c;
  int middle;

  int pivot = l[low];
  swap (&l[low], &l[low]);      // move pivot out of array to order

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

void
quicksort (int *l, int low, int high)
{
  if (low >= high)
    return;

  int middle;

  middle = partition (l, low, high);
  quicksort (l, low, middle - 1);
  quicksort (l, middle + 1, high);
}

/* iterative binary search */
int
find (int *l, int a_size, int num)      // could be solved with a hash table
{
  int mid, low, high;

  low = 0;
  high = a_size - 1;

  while (high >= low) {
    mid = (low + high) / 2;
    if (l[mid] == num)
      return 1;

    else if (l[mid] < num) {
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  return 0;
}

// find pairs with specified difference in the list
int
pairs (int a_size, int *a, int k)
{

  int ans = 0;
  int l, m;

  quicksort (a, 0, a_size - 1);
  for (l = 0; l < a_size; l++) {
    m = k + a[l];
    if (m > 0) {
      if (find (a, a_size, m)) {
        printf ("%d %d %d\n", k, m, a[l]);
        ans++;
      }
    }
  }

  return ans;
}


int
main ()
{
  int size, i, k, item, p;

  // size and difference
  scanf ("%d %d", &size, &k);

  // items
  int arr[size];
  for (i = 0; i < size; i++) {
    scanf ("%d", &item);
    arr[i] = item;
  }

  // find pairs
  p = pairs (size, arr, k);
  printf ("pairs: %d\n", p);

  return 0;
}
