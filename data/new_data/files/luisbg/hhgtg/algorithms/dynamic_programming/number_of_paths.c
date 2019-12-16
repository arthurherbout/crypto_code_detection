#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>

#define XX 5
#define YY 5

typedef struct Point
{
  int x;
  int y;
  bool success;
} Point;

typedef struct Path
{
  Point *p;
  int num_points;
} Path_t;


static bool point_in_cache (Path_t cache[XX][YY], int x, int y);
static Point * get_point_from_cache (Path_t cache[XX][YY], int x, int y);
static void put_point_in_cache (Path_t cache[XX][YY], int x, int y, Point * p);
static void add_point_to_path (Path_t * path, Point * p);


bool
get_path (bool grid[XX][YY], int x, int y, Path_t * path, Path_t cache[XX][YY])
{
  Point * p = (Point *) malloc (sizeof (Point));
  p->x = x;
  p->y = y;

  if (point_in_cache (cache, x, y)) {    // DP cache
    p = get_point_from_cache (cache, x, y);
    return p->success;
  }

  if (x == 0 && y == 0) {
    return true;    // found a path (base case)
  }

  bool success = false;

  if (grid[x - 1][y] && x >= 1) {    // Try Left
    printf ("try point (%d,%d)\n", x - 1, y);
    success = get_path (grid, x - 1, y, path, cache);
  }

  if (!success && (grid[x][y - 1] && y >= 1)) {   // Try Up
    printf ("try point (%d,%d)\n", x, y - 1);
    success = get_path (grid, x, y - 1, path, cache);
  }

  if (success) {
    add_point_to_path (path, p);
  }

  p->success = success;
  printf ("adding point (%d,%d) to cache\n", x, y);
  put_point_in_cache (cache, x, y, p);

  return success;
}

static bool
point_in_cache (Path_t cache[XX][YY], int x, int y)
{
  return (cache[x][y].num_points == 1);
}

static Point *
get_point_from_cache (Path_t cache[XX][YY], int x, int y)
{
  return cache[x][y].p;
}

static void
put_point_in_cache (Path_t cache[XX][YY], int x, int y, Point * p)
{
  cache[x][y].num_points = 1;
  cache[x][y].p = p;

  return;
}

static void
add_point_to_path (Path_t * path, Point * p)
{
  int pos = path->num_points++;
  path->p[pos].x = p->x;
  path->p[pos].y = p->y;

  return;
}

void
show_path (Path_t * path)
{
  for (int pos = 0; pos < path->num_points; pos++) {
    printf ("(%d,%d)\n", path->p[pos].y, path->p[pos].x);
  }
}

int main ()
{
  bool grid[XX][YY];
  int x, y;

  srand (time (NULL));

  for (x = 0; x < XX; x++) {
    for (y = 0; y < YY; y++) {
      if (rand () % 3 > 0)
        grid[x][y] = true;
      else
        grid[x][y] = false;
    }
  }

  for (x = 0; x < XX; x++) {
    for (y = 0; y < YY; y++) {
      printf ("%c ", (grid[x][y])? '.': 'X');
    }
    printf ("\n");
  }
  printf ("\n");

  Path_t cache[XX][YY];
  for (x = 0; x < XX; x++) {
    for (y = 0; y < YY; y++)
      cache[x][y].num_points = 0;
  }


  Path_t * path = (Path_t *) malloc (sizeof (Path_t));
  path->p = (Point *) malloc (XX * YY * sizeof (Point));
  path->num_points = 0;

  get_path (grid, XX - 1, YY - 1, path, cache);
  printf ("\npoints in path: %d\n", path->num_points);

  show_path (path);

  return 0;
}
