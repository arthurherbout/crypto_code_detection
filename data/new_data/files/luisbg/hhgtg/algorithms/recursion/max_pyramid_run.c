/* By starting at the top of the triangle below and moving to adjacent numbers
 * on the row below, the maximum total from top to bottom is 23.
 *
 *       3
 *      7 4
 *     2 4 6
 *    8 5 9 3
 *
 * That is, 3 + 7 + 4 + 9 = 23.
 *
 * Find the maximum total from top to bottom of the triangle below:
 *
 */

#include <stdio.h>


/* recurse down over the triangle adding the two paths from current position */
int recursive_run (int size, int triangle[size][size], int c, int r, int count,
                   int *max)
{
  count += triangle[r][c];

  if (r == size - 1) {
    if (count > *max) {
      *max = count;
      printf (".. %d\n", *max);
    }
    return triangle[r][c];
  }

  recursive_run (size, triangle, c, r+1, count, max);
  recursive_run (size, triangle, c +1, r+1, count, max);

  return *max;
}

/* start the max run */
int find_maximum_run (int size, int triangle[size][size])
{
  int c = 0;
  int max = 0;
  recursive_run (size, triangle, 0, 0, c, &max);

  return max;
}


int main ()
{
  // int size = 4;
  // int triangle[4][4] = { {3, 0, 0, 0},
  //                       {7, 4, 0, 0},
  //                       {2, 4, 6, 0},
  //                       {8, 5, 9, 3}};

  int size = 15;
  int triangle[15][15] = { {75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {95, 64, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {17, 47, 82, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {18, 35, 87, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {20, 4, 82, 47, 65, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {19, 1, 23, 75, 3, 34, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                           {88, 2, 77, 73, 7, 63, 67, 0, 0, 0, 0, 0, 0, 0, 0},
                           {99, 65, 4, 28, 6, 16, 70, 92, 0, 0, 0, 0, 0, 0, 0},
                           {41, 41, 26, 56, 83, 40, 80, 70, 33, 0, 0, 0, 0, 0, 0},
                           {41, 48, 72, 33, 47, 32, 37, 16, 94, 29, 0, 0, 0, 0, 0},
                           {53, 71, 44, 65, 25, 43, 91, 52, 97, 51, 14, 0, 0, 0, 0},
                           {70, 11, 33, 28, 77, 73, 17, 78, 39, 68, 17, 57, 0, 0, 0},
                           {91, 71, 52, 38, 17, 14, 91, 43, 58, 50, 27, 29, 48, 0, 0},
                           {63, 66, 4, 68, 89, 53, 67, 30, 73, 16, 69, 87, 40, 31, 0},
                           {4, 62, 98, 27, 23, 9, 70, 98, 73, 93, 38, 53, 60, 4, 23}};

  int max = find_maximum_run (size, triangle);
  printf ("max: %d\n", max);

  return 0;
}
