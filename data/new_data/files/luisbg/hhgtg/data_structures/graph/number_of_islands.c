/* Given a 2d grid map of '1's (land) and '0's (water), count the number of
 * islands. An island is surrounded by water and is formed by connecting
 * adjacent lands horizontally or vertically. You may assume all four edges
 * of the grid are all surrounded by water.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>


int **
init_map (int gridRowSize, int gridColSize)
{
  int **visited = (int **) malloc (gridRowSize * sizeof (int *));
  for (int y = 0; y < gridRowSize; y++) {
    visited[y] = (int *) malloc (gridColSize * sizeof (int));
    for (int x = 0; x < gridColSize; x++)
      visited[y][x] = 0;
  }

  return visited;
}

void
mark_neighbors_visited (char **grid, int **visited, int y, int x, int h, int w)
{
  // printf ("mark visited %d %d\n", y, x);
  visited[y][x] = 1;

  if (y > 0 && (visited[y - 1][x] == 0) && (grid[y - 1][x] == '1'))     // top
    mark_neighbors_visited (grid, visited, y - 1, x, h, w);

  if (y < (h - 1) && (visited[y + 1][x] == 0) && (grid[y + 1][x] == '1'))       // bottom
    mark_neighbors_visited (grid, visited, y + 1, x, h, w);

  if (x > 0 && (visited[y][x - 1] == 0) && (grid[y][x - 1] == '1'))     // left
    mark_neighbors_visited (grid, visited, y, x - 1, h, w);

  if (x < (w - 1) && (visited[y][x + 1] == 0) && (grid[y][x + 1] == '1'))       // right
    mark_neighbors_visited (grid, visited, y, x + 1, h, w);
}

int
visit_spot (char **grid, int **visited, int y, int x, int h, int w)
{
  int res = 0;

  if (visited[y][x])            // already here before
    return 0;

  if (grid[y][x] == '1') {      // new island discovered
    // printf ("new at %d %d\n", y, x);
    mark_neighbors_visited (grid, visited, y, x, h, w); // mark all island as visited
    res += 1;
  }

  return res;
}

int
num_islands (char **grid, int gridRowSize, int gridColSize)
{
  int **visited = init_map (gridRowSize, gridColSize);
  int count = 0;

  for (int y = 0; y < gridRowSize; y++) {
    for (int x = 0; x < gridColSize; x++) {
      count += visit_spot (grid, visited, y, x, gridRowSize, gridColSize);
    }
  }

  return count;
}


int
main ()
{
  int islands;
  srand (time (NULL));
  char **grid = (char **) malloc (4 * sizeof (char *));
  for (int y = 0; y < 4; y++) {
    grid[y] = (char *) malloc (5 * sizeof (char));
    for (int x = 0; x < 5; x++)
      grid[y][x] = (rand () % 3) < 1 ? '1' : '0';
  }

  // show map
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 5; x++)
      printf ("%c ", grid[y][x]);
    printf ("\n");
  }

  // count islands
  islands = num_islands (grid, 4, 5);
  printf ("number of islands: %d\n", islands);

  return 0;
}
