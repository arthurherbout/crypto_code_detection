/* CodeJam solution Hiking Deer in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

/* Solution for small/medium:  Herbert can decide to cross the finish just
   after any one of the maximum ten hikers.  For each of the others, we can
   compute how often they cross the finish line before.  This gives
   us the number of encounters.

   Following the fastest one gives an immediate upper bound of nine
   encounters.  Thus we can exit early whenever this number is exceeded.  */

struct Hiker
{
  int64_t x;
  int64_t invV;
};

/* Compute time for crossing the finish k times for a given hiker.  */
inline int64_t
crossingTime (const Hiker& h, int64_t k)
{
  return (k * 360 - h.x) * h.invV;
}

/* Compute the number of encounters if we follow hiker number i.  */
int
encountersFollowing (const std::vector<Hiker>& hikers, unsigned i)
{
  int res = 0;
  const int64_t ourTime = crossingTime (hikers[i], 1);

  /* Count how often we are overtaken by others.  */
  for (unsigned j = 0; j < hikers.size (); ++j)
    if (i != j)
      {
        for (int64_t k = 2; ; ++k)
          {
            const int64_t t = crossingTime (hikers[j], k);
            if (ourTime < t)
              break;

            ++res;
            if (res > 9)
              return -1;
          }
      }

  /* Add how often we overtake others.  */
  for (unsigned j = 0; j < hikers.size (); ++j)
    if (i != j)
      {
        const int64_t t = crossingTime (hikers[j], 1);
        if (ourTime < t)
          ++res;
      }

  return res;
}

/* Solve a single case.  */
int
solve_case ()
{
  int n;
  std::cin >> n;

  std::vector<Hiker> hikers;
  for (int i = 0; i < n; ++i)
    {
      int d, h, m;
      std::cin >> d >> h >> m;
      for (int j = 0; j < h; ++j)
        hikers.push_back (Hiker{d, m + j});
    }

  int bestSoFar = -1;
  for (unsigned i = 0; i < hikers.size (); ++i)
    {
      const int cur = encountersFollowing (hikers, i);
      if (cur >= 0 && (bestSoFar == -1 || cur < bestSoFar))
        bestSoFar = cur;
    }
  assert (bestSoFar >= 0);

  return bestSoFar;
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
