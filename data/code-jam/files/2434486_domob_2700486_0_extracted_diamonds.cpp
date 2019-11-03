/* CodeJam solution diamonds in C++ by domob.  */

/* Note that if a "triangle number" of diamonds falls, they will *always*
   form a triangle.  After that, further diamonds will slide down either side
   to fill up the next triangle from both sides until it is complete.  So
   after the final full triangle is formed, we only have to try out the
   possibilities for how many could fall left and how many of the remaining
   right.  Probably an even faster solution is possible (calculating the
   probability directly from the number of remaining diamonds and the
   size of the triangle, but I probably don't care about that.
   
   Triangle number means here that we have a "pyramid" formed with an odd
   number of rows.  Thus the contained numbers are:

   1
   1 + 2 + 3 = 6
   1 + 2 + 3 + 4 + 5 = 15
   ...  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <vector>

typedef std::vector<double> cacheRow;
typedef std::vector<cacheRow> cacheT;

cacheT cache;

/* Calculate binomial coefficient.  We use Pascal's triangle for that and
   cache already calculated values.  */
double
binom (unsigned n, unsigned k)
{
  assert (k <= n);

  while (cache.size () <= n)
    {
      const int curN = cache.size ();
      cache.push_back (cacheRow (curN + 1, 1));
      for (int curK = 1; curK < curN; ++curK)
        cache.back ()[curK] = binom (curN - 1, curK - 1)
                              + binom (curN - 1, curK);
    }

  const double res = cache[n][k];
#ifndef NDEBUG
  if (n == k || k == 0)
    assert (std::abs (res - 1.0) < 1.0e-9);
  else
    assert (std::abs (res - binom (n - 1, k - 1) - binom (n - 1, k)) < 1.0e-9);
#endif

  return res;
}

/* Solve a single case.  */
void
solve_case ()
{
  int n, x, y;
  scanf ("%d %d %d\n", &n, &x, &y);

  /* Find the largest triangle smaller or equal to n.  */
  int t, sum;
  sum = 1;
  t = 1;
  while (sum + t + 1 + t + 2 <= n)
    {
      ++t;
      sum += t;
      ++t;
      sum += t;
    }
  assert (sum <= n && sum + t + 1 + t + 2 > n);

  /* See if our position is already inside.  */
  if (std::abs (x) + std::abs (y) <= t)
    {
      printf ("1.0");
      return;
    }

  /* See if the position is too far away.  */
  if (std::abs (x) + std::abs (y) > t + 2)
    {
      printf ("0.0");
      return;
    }
  assert (std::abs (x) + std::abs (y) == t + 1);
  
  /* Also not possible is at the top, since then we would need already
     a full next triangle!  */
  if (x == 0)
    {
      assert (y == t + 1);
      printf ("0.0");
      return;
    }
  assert (y < t + 1);

  /* We're right at the current layer.  See how many diamonds of the
     remaining we need at least on the right side in order to have the
     position covered.  */
  const int needRight = y + 1;
  const int remaining = n - sum;
  assert (needRight >= 0 && remaining >= 0);

  /* See how likely it is to have them, following a Binomial distribution.
     We need at least needRight out of remaining hits.  Furthermore, it is also
     ok enough go to the other direction, making it full, until all others
     must surely go to this one instead.  */
  double p = 0.0;
  for (int k = 0; k <= remaining; ++k)
    {
      bool ok = false;
      if (k >= needRight)
        ok = true;
      if ((remaining - k) - (t + 1) >= needRight)
        ok = true;

      if (ok)
        p += binom (remaining, k);
    }
  p *= std::pow (0.5, remaining);

  printf ("%.12f", p);
}

/* Main routine, handling the different cases.  */
int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    {
      printf ("Case #%d: ", i);
      solve_case ();
      printf ("\n");
    }

  return EXIT_SUCCESS;
}
