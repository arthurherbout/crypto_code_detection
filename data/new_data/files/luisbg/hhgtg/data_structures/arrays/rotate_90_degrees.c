/* Given an NxN matrix, rotate it by 90 degrees in place.                     */

#include <stdlib.h>
#include <stdio.h>


/* print the two-dimensional array of numbers */
void
show_array (int size, int arr[size][size])
{
  int x, y;
  for (x = 0; x < size; x++) {
    for (y = 0; y < size; y++) {
      printf ("%d ", arr[x][y]);
    }
    printf ("\n");
  }

  printf ("\n");
}

/* rotation in place to the right */
void
rotate_right (int n, int matrix[n][n])
{
  int layer, first, last, offset, top, i;
  for (layer = 0; layer < (n / 2); layer++) {
    first = layer;
    last = n - 1 - layer;

    for (i = first; i < last; i++) {
      offset = i - first;
      // save top
      top = matrix[first][i];

      // left -> top
      matrix[first][i] = matrix[last - offset][first];

      //bottom -> left
      matrix[last - offset][first] = matrix[last][last - offset];

      // right -> bottom
      matrix[last][last - offset] = matrix[i][last];

      // top -> right
      matrix[i][last] = top;
    }
  }
}

/* rotate to the left using auxiliary memory */
void
rotate_left (int size, int arr[size][size], int new[size][size])
{
  int x, y;
  int new_y;
  for (x = 0; x < size; x++) {
    for (y = 0; y < size; y++) {
      if (x <= size)
        new_y = (size - 1) - x;
      else
        new_y = x - (size - 1);

      new[x][y] = arr[y][new_y];
    }
  }

  printf ("rotate left\n");
}


int
main ()
{
  printf ("original\n");
  int arr[4][4] = {{0, 1, 2, 3}, {4, 5, 6, 7}, {8, 9, 10, 11},
                   {12, 13, 14, 15}};
  int rot[4][4];

  show_array (4, arr);

  rotate_left (4, arr, rot);
  show_array (4, rot);

  printf ("rotate right\n");
  rotate_right (4, arr);
  show_array (4, arr);

  return 0;
}
