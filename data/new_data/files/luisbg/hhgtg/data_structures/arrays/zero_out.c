/* Write an algorithm such that if an element in an MxN matrix is 0, its      */
/* row and column are set to 0.                                               */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


/* zero out the rows and columns with a 0 in them */
void
zero_out (int m, int n, int matrix[m][n])
{
  int x, y;
  bool *col_zero = (bool *) calloc (n, sizeof (bool));
  bool *row_zero = (bool *) calloc (m,  sizeof (bool));

  for (x = 0; x < m; x++)
    for (y = 0; y < n; y++)
        if (matrix[x][y] == 0)
          row_zero[y] = col_zero[x] = true;

  for (x = 0; x < m; x++)
    for (y = 0; y < n; y++)
      if (col_zero[x] || row_zero[y])
        matrix[x][y] = 0;

  free (row_zero);
  free (col_zero);
}

/* print two-dimensional array of numbers */
void
show_array (int m, int n, int arr[m][n])
{
  int x, y;
  for (x = 0; x < m; x++) {
    for (y = 0; y < n; y++) {
      printf ("%2d  ", arr[x][y]);
    }
    printf ("\n");
  }

  printf ("\n");
}


int
main ()
{
  int matrix[6][5] = { {1, 2, 3, 4, 5},
  {6, 0, 8, 9, 10},
  {11, 12, 13, 14, 15},
  {16, 0, 18, 19, 20},
  {21, 22, 23, 0, 25},
  {26, 27, 28, 29, 30} };
  show_array (6, 5, matrix);

  zero_out (6, 5, matrix);
  show_array (6, 5, matrix);

  return 0;
}
