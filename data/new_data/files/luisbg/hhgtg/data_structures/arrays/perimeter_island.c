/* You are given a map in form of a two-dimensional integer grid where 1
 * represents land and 0 represents water. Grid cells are connected
 * horizontally/vertically. The grid is completely surrounded by water, and
 * there is exactly one island (i.e., one or more connected land cells).
 *
 * Determine the perimeter of the island.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int
is_a_perimeter (int **grid, int y, int x, int height, int width)
{
  if (x < 0 || y < 0 || y >= height || x >= width)
    return 1;

  if (grid[y][x] == 0)
    return 1;
  else
    return 0;
}

int
island_perimeter (int **grid, int height, int width)
{
  int perimeter = 0;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      if (grid[y][x]) {
        // left
        perimeter += is_a_perimeter (grid, y, x - 1, height, width);
        // top
        perimeter += is_a_perimeter (grid, y - 1, x, height, width);
        // right
        perimeter += is_a_perimeter (grid, y, x + 1, height, width);
        // bottom
        perimeter += is_a_perimeter (grid, y + 1, x, height, width);
      }
    }
  }

  return perimeter;
}

void
print_grid (int **grid, int height, int width)
{
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      printf ("%d ", grid[y][x]);
    }
    printf ("\n");
  }

}

int
main ()
{
  srand (time (NULL));

  int height = 4;
  int width = 4;
  int **grid = (int **) malloc (height * sizeof (int *));
  for (int y = 0; y < height; y++) {
    grid[y] = (int *) malloc (width * sizeof (int));
    for (int x = 0; x < width; x++) {
      grid[y][x] = (rand () % 3 > 0) ? 0 : 1;
    }
  }

  print_grid (grid, height, width);
  printf ("perimeter of size %d\n", island_perimeter (grid, height, width));

  for (int y = 0; y < height; y++)
    free (grid[y]);
  free (grid);

  return 0;
}
