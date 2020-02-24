/* CodeJam solution Noisy Neighbours in C++ by domob.  */

#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

/* Solution for small:  Just try it out.  2^16 is not too many possibilities
   to try for each run.  */
static const int MAX_SZ = 16;
bool filled[MAX_SZ][MAX_SZ];

/* Recursively place tenants on the map.  Keep track of the total
   unhappiness produced so far.  */
int
recurse (int r, int c, int from, int n, int unhappy)
{
  assert (n >= 0);
  if (n == 0)
    return unhappy;

  int bestSoFar = -1;
  for (int comb = from; comb <= r * c - n; ++comb)
    {
      const int i = comb / c;
      const int j = comb % c;

      if (!filled[i][j])
        {
          filled[i][j] = true;
          int unh = 0;
#define CHECKDIR(dr, dc) \
  if (i + dr >= 0 && i + dr < r \
        && j + dc >= 0 && j + dc < c \
        && filled[i + dr][j + dc]) \
    ++unh;
          CHECKDIR(-1, 0)
          CHECKDIR(1, 0)
          CHECKDIR(0, -1)
          CHECKDIR(0, 1)
          const int cur = recurse (r, c, comb + 1, n - 1, unhappy + unh);
          if (bestSoFar == -1 || bestSoFar > cur)
            bestSoFar = cur;
          filled[i][j] = false;
        }
    }

  assert (bestSoFar >= 0);
  return bestSoFar;
}

/* Solve a single case.  */
int
solve_case ()
{
  int r, c, n;
  std::cin >> r >> c >> n;

  std::fill (reinterpret_cast<bool*> (filled),
             reinterpret_cast<bool*> (filled) + MAX_SZ * MAX_SZ, false);

  return recurse (r, c, 0, n, 0);
}

/* Main routine, handling the different cases.  */
int
main ()
{
  unsigned n;

  std::cin >> n;
  for (unsigned i = 1; i <= n; ++i)
    std::cout << "Case #" << i << ": " << solve_case () << std::endl;

  return EXIT_SUCCESS;
}
