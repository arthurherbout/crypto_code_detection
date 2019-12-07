/* Given an index k, return the kth row of the Pascal's triangle. */
/* https://en.wikipedia.org/wiki/Pascal's_triangle */

#include <stdio.h>
#include <stdlib.h>


void
populate_pascal (int size, int grid[size][size])
{
  // first column is all 1
  for (int c = 0; c < size; c++)
    grid[c][0] = 1;

  for (int r = 1; r < size; r++) {
    for (int c = 1; c < size; c++) {
      grid[r][c] = grid[r - 1][c - 1] + grid[r - 1][c];
    }
  }
}

void
print_grid (int size, int grid[size][size])
{
  for (int r = 0; r < size; r++) {
    for (int c = 0; c < size; c++) {
      printf ("%d", grid[r][c]);
    }
    printf ("\n");
  }
}

int *
get_row (int rowIndex, int *returnSize)
{
  int pasc_size = rowIndex + 1;
  int pasc[pasc_size][pasc_size];
  for (int r = 0; r < pasc_size; r++)
    for (int c = 0; c < pasc_size; c++)
      pasc[r][c] = 0;

  populate_pascal (pasc_size, pasc);

  // print_grid (pasc_size, pasc);

  int *ret = (int *) malloc (pasc_size * sizeof (int));
  for (int c = 0; c < pasc_size; c++)
    ret[c] = pasc[rowIndex][c];

  *returnSize = pasc_size;
  return ret;
}

int
pasc_position (int row, int column)
{
  if (column == 0)
    return 1;

  if (row == 0)
    return 0;

  return pasc_position (row - 1, column - 1) + pasc_position (row - 1, column);
}

int *
get_row_recursive (int rowIndex, int *returnSize)
{
  int pasc_size = rowIndex + 1;
  int *row = (int *) malloc (pasc_size * sizeof (int));

  for (int c = 0; c < pasc_size; c++)
    row[c] = pasc_position (rowIndex, c);

  *returnSize = pasc_size;
  return row;
}

int *
get_row_direct (int rowIndex, int *returnSize)
{
  int pasc_size = rowIndex + 1;
  int *row = (int *) calloc (pasc_size, sizeof (int));

  row[0] = 1;
  for (int c = 1; c < pasc_size; c++)
    for (int d = c; d >= 1; d--)
      row[d] += row[d - 1];

  *returnSize = pasc_size;
  return row;
}

void
print_pascal_row (int r)
{
  int size;
  int *row;

  if (r % 2 == 0)
    row = get_row_recursive (r, &size);
  else
    row = get_row_direct (r, &size);

  for (int c = 0; c < size; c++) {
    printf ("%d ", row[c]);
  }
  printf ("\n");

  free (row);
}

int
main ()
{
  for (int c = 1; c <= 12; c++) {
    print_pascal_row (c);
  }

  return 0;
}
