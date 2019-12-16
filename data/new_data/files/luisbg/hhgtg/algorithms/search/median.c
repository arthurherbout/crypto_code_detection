#include <stdio.h>
#include <stdlib.h>


int find_median (int *l, int size);
static void print_list (int *l, int len);
static void swap (int *a, int *b);
static int partition (int *l, int low, int high);


static void
print_list (int *l, int len)
{
  int c;
  for (c = 0; c < len; c++)
    printf ("%d ", l[c]);
  printf ("\n");
}

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

  int k = low + 1;
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
find_median (int *l, int size)
{
  int middle;
  int median = size / 2;
  int low = 0;
  int high = size - 1;
  // int steps = 1;

  middle = partition (l, low, high);
  while (middle != median) {    // partition aproximating towards median
    // printf ("steps: %d\n", steps++);
    if (middle > median) {
      high = middle;
      middle = partition (l, low, middle - 1);
    } else {
      low = middle;
      middle = partition (l, middle + 1, high);
    }
  }

  return l[median];
}


int
main ()
{
  int size_list;
  int c;
  int median;

  size_list = rand () % 50;
  int *list = (int *) malloc (size_list * sizeof (int));

  for (c = 0; c < size_list; c++) {
    list[c] = rand () % 200;
  }
  print_list (list, size_list);

  median = find_median (list, size_list);
  printf ("\nmedian is: %d\n", median);

  return 0;
}
