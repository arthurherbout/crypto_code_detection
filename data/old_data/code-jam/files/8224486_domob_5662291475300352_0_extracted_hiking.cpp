/* CodeJam solution Hiking Deer in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

/* For small/small, the situation is easy:  There can only ever be at most
   two hikers.  If there are actually zero or one, then Herbert can always
   "win" without any encounters.  For two hikers, he can always go as fast
   as the faster of them, which wins for him with at most one encounter.

   Zero encounters are possible if the hiker "in front" of him (with lower
   starting position) does not encounter the other hiker before reaching
   the target.  The first encounter of the hikers:

      x_1 + t v_1 = x_2 + t v_2 - 360 k
  
   where k is a natural number.  If this has a solution for some k
   and with x_1 + t v_1 <= 360, then the answer is one.  Otherwise, zero.
   Note that k = 0 means that they encounter before hitting Herbert, so this
   is excluded and only k >= 1 is important.

   Solving the equation yields:

      t = (x_2 - x_1 - 360 k) / (v_1 - v_2)

   Actually, only k = 1 is interesting.
*/

struct Hiker
{
  int64_t x;
  int64_t invV;
};

/* Sort by speed.  I. e., the slower hikers are first.  */
inline bool
operator< (const Hiker& a, const Hiker& b)
{
  if (a.invV != b.invV)
    return a.invV > b.invV;
  return a.x < b.x;
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

  std::sort (hikers.begin (), hikers.end ());

  if (hikers.size () <= 1)
    return 0;

  /* If both hikers have the same speed, it is possible.  */
  if (hikers[0].invV == hikers[1].invV)
    return 0;
  assert (hikers[0].invV > hikers[1].invV);

  /* Otherwise, the question is whether the slower hiker reaches the goal
     before the faster hiker reaches it the second time.
          
        t0 = (360 - x0) / 360 * invV0
        t1 = (2 * 360 - x1) / 360 * invV1
  */

  const int64_t t0 = (360 - hikers[0].x) * hikers[0].invV;
  const int64_t t1 = (2 * 360 - hikers[1].x) * hikers[1].invV;
  if (t0 >= t1)
    return 1;
  return 0;
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
