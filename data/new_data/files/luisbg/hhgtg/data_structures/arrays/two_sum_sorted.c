/* Given an array of integers that is already sorted in ascending order, find
 * two numbers such that they add up to a specific target number.
 *
 */

#include <stdio.h>
#include <stdlib.h>


int
binary_search (int *numbers, int numberSize, int key, int invalid)
{
  int start = 0;
  int end = numberSize - 1;
  int check;

  while (start <= end) {
    check = (start + end) / 2;
    // printf ("need %d got %d\n", key, numbers[check]);
    if (numbers[check] == key && check != invalid)
      return check;
    else if (numbers[check] <= key) {
      start = check + 1;
    } else {
      end = check - 1;
    }
  }

  return -1;
}

int *
two_sum (int *numbers, int numbers_size, int target)
{
  int search;
  for (int a = 0; a < numbers_size; a++) {
    search = binary_search (numbers, numbers_size, target - numbers[a], a);
    // printf ("a %d search %d\n", a, search);
    if (search >= 0) {
      int *ret = (int *) malloc (2 * sizeof (int));
      ret[0] = a;
      ret[1] = search;

      return ret;
    }
  }

  return NULL;
}

int *
two_sum_direct (int *numbers, int numbers_size, int target)
{
  int start = 0;
  int end = numbers_size - 1;
  int tmp;

  while (start < end) {
    // printf ("s %d  e %d\n", start, end);
    tmp = numbers[start] + numbers[end];
    if (tmp == target) {
      int *ret = (int *) malloc (2 * sizeof (int));
      ret[0] = start;
      ret[1] = end;

      return ret;
    }

    else if (tmp < target)
      start++;
    else
      end--;
  }

  return NULL;
}


int
main ()
{
  int nums[7] = { 2, 1, 11, 15, 17, 20, 23 };

  int *ts = two_sum_direct (nums, 7, 31);
  printf ("indices that sum 31 in the following array:\n");
  for (int i = 0; i < 7; i++)
    printf ("%d ", nums[i]);
  printf ("\nwe use indices %d and %d\n", ts[0], ts[1]);

  free (ts);
  return 0;
}
