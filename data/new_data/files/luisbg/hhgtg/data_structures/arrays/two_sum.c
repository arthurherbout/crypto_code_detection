/* Given an array of integers, return indices of the two numbers such that they
 * add up to a specific target.
 *
 * You may assume that each input would have exactly one solution, and you may
 * not use the same element twice.
 *
 */

#include <stdio.h>
#include <stdlib.h>


// This is O(n^2).
// If we search for (target - nums[a]) in a hashtable, it is O(n)
int *
two_sum (int *nums, int numsSize, int target)
{
  int diff;

  for (int a = 0; a < numsSize; a++) {
    diff = target - nums[a];
    for (int b = a + 1; b < numsSize; b++)
      if (nums[b] == diff) {
        int *ret = (int *) malloc (2 * sizeof (int));
        ret[0] = a;
        ret[1] = b;
        return ret;
      }
  }

  return NULL;
}

int
main ()
{
  int nums[7] = { 2, 1, 11, 15, 3, 27, 20 };

  int *ts = two_sum (nums, 7, 31);
  printf ("indices that sum 14 in the following array:\n");
  for (int i = 0; i < 7; i++)
    printf ("%d ", nums[i]);
  printf ("\nwe use indices %d and %d\n", ts[0], ts[1]);

  free (ts);
  return 0;
}
