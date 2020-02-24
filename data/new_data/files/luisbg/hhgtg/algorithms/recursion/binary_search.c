/* Recursive Binary Search */

#include <stdio.h>

int binary_search (int *array, int lower, int upper, int target);


/* binary search: recurse over half of the slice where the value should be until found */
int
binary_search (int *array, int lower, int upper, int target)
{
  int middle = (lower + upper) / 2;

  if (target == array[middle])  // base case
    return middle;

  // ERROR checks
  if (lower == middle)
    return -1;                  // element not in list
  if (lower > upper)
    return -2;                  // limits out of order
  if (array[lower] > array[upper])
    return -3;                  // list not sorted

  if (target < array[middle])   // recursive cases
    return binary_search (array, lower, middle - 1, target);
  else
    return binary_search (array, middle + 1, upper, target);
}


int
main ()
{
  int array[10] = { 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 };
  int index;

  index = binary_search (array, 0, 9, 16);
  printf ("16 is in position: %d\n", index);

  index = binary_search (array, 0, 9, 100);
  if (index == -1)
    printf ("100 is not in the list\n");

  index = binary_search (array, 9, 0, 10);
  if (index == -2)
    printf ("limits out of order\n");

  return 0;
}
