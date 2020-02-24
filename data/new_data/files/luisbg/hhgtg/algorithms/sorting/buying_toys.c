/* Maximize the number of toys that can be purchased. */
/* N toy items, tagged with their prices and only K available */

#include <stdio.h>
#include <stdlib.h>


void
print_list (int *l, int len)
{
  int c;
  for (c = 0; c < len; c++) {
    printf ("%03d ", l[c]);
  }
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
get_pivot (int low, int high)
{
  return (low + high) / 2;
}

static int
partition (int *l, int low, int high)
{
  int c;
  int middle;

  int k = get_pivot (low, high);
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

// get the number of toys that can be purchased with the given amount of money
int
purchased_toys (int num_items, int *toys, int money)
{
  int i, purchased = 0;

  quicksort (toys, 0, num_items - 1);

  for (i = 0; i < num_items; i++) {
    if (money > 0 && toys[i] < money) {
      money -= toys[i];
      purchased++;

    } else
      break;
  }

  return purchased;
}

int
main ()
{
  int num_items = 15;
  int toys[15];
  int money;

  int i;
  for (i = 0; i < num_items; i++) {
    toys[i] = rand () % 1000;
  }

  print_list (toys, num_items);

  money = 0;
  printf ("%d toys can be bought with %d credits\n",
      purchased_toys (num_items, toys, money), money);

  money = 100;
  printf ("%d toys can be bought with %d credits\n",
      purchased_toys (num_items, toys, money), money);

  money = 2000;
  printf ("%d toys can be bought with %d credits\n",
      purchased_toys (num_items, toys, money), money);

  money = 10000;
  printf ("%d toys can be bought with %d credits\n",
      purchased_toys (num_items, toys, money), money);


  return 0;
}
