/* CodeJam solution smooth in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

/* This really sounds like dynamic programming to me...  So base it all on the
   cache-able recursive function:
     cost to make the sub-image [k .. end] smooth when there is
     pixel with value a to the left of it?  */

int d, i, m, n;
int pixels[300];


/* Find that recursive relation.  Try to do something to/at pixel
   k, such that the image [k .. end] becomes smooth when we have
   the pixel-value a to the left of k.  a = -1 means that there's
   no pixel there.

   To avoid infinite recursion, notice that there's a critical number
   of elements to insert (maxins) such that deleting all elements would
   be cheaper than inserting those many and thus the optimal solution
   can not consist of more than maxins inserts.  Do only insert until
   this number gets exceeded.  */

int cache[257][300];
int inscache[257][300];

int
findCost (int a, int k, int curins, int maxins)
{
  int res, cur;

  if (k == n)
    return 0;
  assert (k < n);

  if (cache[a + 1][k] != -1 && inscache[a + 1][k] <= curins)
    return cache[a + 1][k];

  // Try deleting current pixel.
  cur = d + findCost (a, k + 1, 0, maxins);
  res = cur;

  for (int newVal = 0; newVal <= 255; ++newVal)
    {
      if (a != -1 && std::abs (a - newVal) > m)
        continue;
      
      // Try altering current pixel.  Includes no change at all.
      int newCost = std::abs (pixels[k] - newVal);
      cur = newCost + findCost (newVal, k + 1, 0, maxins);
      if (cur < res)
        res = cur;

      // Try inserting before pixel k.
      if (curins < maxins)
        {
          cur = i + findCost (newVal, k, curins + 1, maxins);
          if (cur < res)
            res = cur;
        }
    }

  cache[a + 1][k] = res;
  inscache[a + 1][k] = curins;
  return res;
}


/* Solve a single case.  */

void
solve_case ()
{
  std::fill ((int*)cache, ((int*)cache) + sizeof (cache) / sizeof (int), -1);

  scanf ("%d %d %d %d\n", &d, &i, &m, &n);
  assert (sizeof (int) * n < sizeof (pixels));
  for (int k = 0; k < n; ++k)
    {
      scanf ("%d", &pixels[k]);
      assert (pixels[k] >= 0 && pixels[k] <= 255);
    }

  int maxins;
  if (i > 0)
    maxins = 1 + (d * n) / i;
  else
    maxins = 255 * n;
  printf ("%d", findCost (-1, 0, 0, maxins));
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
