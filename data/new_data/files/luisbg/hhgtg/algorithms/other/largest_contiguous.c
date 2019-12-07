/* You are given an array of integers (both positive and negative). Find */
/* the contiguous sequence with the largest sum. Return the sum */

#include <stdio.h>
#include <stdlib.h>


int
find_largest_contiguous (int *array, int len)
{
  int sum, tmp_sum;
  // int start, end;

  int c, n;
  for (c = 0; c < len; c++) {
    tmp_sum = array[c];
    for (n = c + 1; n < len; n++) {
      tmp_sum += array[n];

      if (tmp_sum < 0)
        break;

      if (tmp_sum > sum) {
        // start = c;
        // end = n;
        sum = tmp_sum;
      }
    }
  }

  //  int *res = (int *) malloc (2 * sizeof (int));
  //  res[0] = start;
  //  res[1] = end;

  return sum;
}

int
find_largest_faster (int *array, int len)
{
  int sum = 0;
  int maxsum = 0;

  int c;
  for (c = 0; c < len; c++) {
    sum += array[c];

    if (maxsum < sum) {
      maxsum = sum;
    } else if (sum < 0) {
      sum = 0;
    }
  }
  return maxsum;
}

int
main ()
{
  int len = 8;
  int arr[8] = { 2, 3, -8, -1, 2, 4, -2, 3 };

  int cont;
  cont = find_largest_faster (arr, len);

  printf ("%d\n", cont);

  return 0;
}
