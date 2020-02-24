/* CodeJam solution nile in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

struct Cell
{

  bool free;

  Cell () = default;
  Cell (const Cell&) = default;
  Cell& operator= (const Cell&) = default;

};
Cell cells[102][502];

/* Find one path given the current position and the target height.  */
static bool
findPath (unsigned x, unsigned y, unsigned targetH, unsigned lastDir)
{
  if (!cells[x][y].free)
    return false;

  cells[x][y].free = false;
  if (y == targetH)
    return true;
  assert (y < targetH);

  static const int dirs[][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
  for (unsigned d = lastDir + 3; d < lastDir + 7; ++d)
    {
      const auto& dd = dirs[d % 4];
      const unsigned xx = x + dd[0];
      const unsigned yy = y + dd[1];
      if (cells[xx][yy].free)
        return findPath (xx, yy, targetH, d % 4);
    }

  cells[x][y].free = true;
  return false;
}

/* Solve a single case.  */
static void
solve_case ()
{
  unsigned w, h, b;
  scanf ("%u %u %u\n", &w, &h, &b);

  for (auto& r : cells)
    for (auto& c : r)
      c.free = false;

  for (unsigned i = 1; i <= w; ++i)
    for (unsigned j = 1; j <= h; ++j)
      cells[i][j].free = true;

  for (unsigned i = 0; i < b; ++i)
    {
      unsigned x0, y0, x1, y1;
      scanf ("%u %u %u %u\n", &x0, &y0, &x1, &y1);
      ++x0; ++y0; ++x1; ++y1;

      for (unsigned x = x0; x <= x1; ++x)
        for (unsigned y = y0; y <= y1; ++y)
          cells[x][y].free = false;
    }

#if 0
  printf ("\n\n");
  for (unsigned i = h; i >= 1; --i)
    {
      for (unsigned j = 0; j <= w + 1; ++j)
        printf ("%c", cells[j][i].free ? ' ' : '*');
      printf ("\n");
    }
  printf ("\n\n");
#endif // Debug print.

  /* Try to find paths keeping "as left as possible" until no more paths
     are available.  This is inspired by the Ford-Fulkerson algorithm and
     (hopefully?) correct in this special case.  */
  unsigned res = 0;
  bool found;
  do
    {
      found = false;
      for (unsigned i = 1; i <= w; ++i)
        if (findPath (i, 1, h, 0))
          {
            found = true;
            ++res;
            break;
          }
    }
  while (found);

  printf ("%u", res);
}

/* Main routine, handling the different cases.  */
int
main ()
{
  unsigned n;

  //std::cin >> n;
  scanf ("%u\n", &n);
  for (unsigned i = 1; i <= n; ++i)
    {
      //std::cout << "Case #" << i << ": ";
      printf ("Case #%u: ", i);

      solve_case ();

      //std::cout << std::endl;
      printf ("\n");
    }

  return EXIT_SUCCESS;
}
