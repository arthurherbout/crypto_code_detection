/* Given an array of integers, every element appears twice except for one.
 * Find that single one.
 */

#include <stdio.h>

int
single_number (int *nums, int numsSize)
{
  int tmp = 0;
  for (int c = 0; c < numsSize; c++) {
    tmp ^= nums[c];
  }

  return tmp;
}

int
main ()
{
  int arr[7] = { 1, 2, 3, 4, 3, 2, 1 };

  printf ("array is: ");
  for (int c = 0; c < 7; c++)
    printf ("%d ", arr[c]);
  printf ("\n");

  printf ("single number is: %d\n", single_number (arr, 7));

  return 0;
}
