/* Find the contiguous subarray within an array (containing at least one
 * number) which has the largest sum.
 */

#include <stdio.h>


static inline int
max (int a, int b)
{
  return (a >= b) ? a : b;
}

int
max_sub_array (int *nums, int numsSize)
{
  int sub_sum = 0;
  int max_sub_sum = nums[0];

  for (int c = 0; c < numsSize; c++) {
    sub_sum += nums[c];
    max_sub_sum = max (sub_sum, max_sub_sum);

    if (sub_sum < 0)
      sub_sum = 0;
  }

  return max_sub_sum;
}

void
print_array (int *nums, int numsSize)
{
  printf ("[");
  for (int c = 0; c < (numsSize - 1); c++) {
    printf ("%2d, ", nums[c]);
  }

  printf ("%2d]\n", nums[numsSize - 1]);
}

int
main ()
{
  int arr_a[10] = { -2, 1, -3, 4, -1, 2, 1, -5, 4, -1 };
  int arr_b[10] = { -3, -2, -1, -2, -3, -4, -5, -6, -7, -8 };
  int arr_c[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

  print_array (arr_a, 10);
  printf ("max sub array is: %d\n", max_sub_array (arr_a, 10));

  print_array (arr_b, 10);
  printf ("max sub array is: %d\n", max_sub_array (arr_b, 10));

  print_array (arr_c, 10);
  printf ("max sub array is: %d\n", max_sub_array (arr_c, 10));

  return 0;
}
