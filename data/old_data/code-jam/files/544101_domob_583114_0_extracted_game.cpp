/* CodeJam solution game in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

/* One loses, if and only if both numbers on the board are the same
   (in the current turn).  Otherwise, you will of course only want to
   subtract the smaller one off the larger.  This takes place until
   the larger one is reduced to less than the smaller, effectively
   calculating the remainder.

   So my choice as player is to directly do this or rather stop one
   step before reaching the remainder, so that either I or my opponent
   will be faced with B, A%B (if A was larger).  Thus I need to find out
   whether that situation is a winning one or otherwise a losing one.  I can
   choose my turn to get the winning one if A/B >= 2.  Otherwise I'm forced
   to accept what I have and do the remainder.

   This makes for an easy way to check a position.  */

typedef int64_t intT;


/* Check whether a position wins or loses.  */

bool
winningPosition (intT A, intT B)
{
  if (A == B)
    return false;

  if (A < B)
    return winningPosition (B, A);
  assert (A > B);

  if (A / B >= 2)
    return true;

  return !winningPosition (B, A % B);
}


/* Solve a single test case.  */

void
solve_case ()
{
  intT A1, A2, B1, B2;
  scanf ("%lld %lld %lld %lld", &A1, &A2, &B1, &B2);

  intT cnt = 0;
  for (intT A = A1; A <= A2; ++A)
    for (intT B = B1; B <= B2; ++B)
      if (winningPosition (A, B))
        ++cnt;

  printf ("%lld", cnt);
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
