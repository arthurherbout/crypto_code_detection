/* CodeJam solution cards in C++ by domob.  */

/* Do this with dynamic programming on the recursion:
   result(missing) which gives the average number of packs to buy when
   still missing a certain number of cards.
   This one can be found by buying a pack and trying all possibilities,
   how many new cards there could be found in it.  */

#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>

typedef double realT;

static const int MAX_NC = 41;
static const int LIMIT_BOUND = 1000;

static int n, c;
static realT neededPacks[MAX_NC][LIMIT_BOUND];
static realT probForNew[MAX_NC][MAX_NC][MAX_NC][MAX_NC];


/* Probability to hit a number of new cards when a certain number of cards
   was still missing.
   This is another DP, with the recursion:
   prob(n, c, missing, newOnes) where n/c are the probably reduced
   N/C values.  */

static realT
getNewProb (const int myN, const int myC, const int missing, const int newOnes)
{
  if (newOnes < 0)
    return 0.0;

  assert (myN >= 0);
  assert (myC >= 0);
  assert (missing >= 0);
  assert (newOnes >= 0);

  if (newOnes > myN)
    return 0.0;
  if (missing > myC)
    return 0.0;
  if (myN == 0)
    return (newOnes == 0) ? 1.0 : 0.0;

  if (probForNew[myN][myC][missing][newOnes] < 0.0)
    {
      realT res = 0.0;

      /* Draw one.  It can either be a new one or not.  */
      const realT probNew = realT (missing) / realT (myC);
      const realT probNoNew = 1.0 - probNew;

      res = probNew * getNewProb (myN - 1, myC - 1, missing - 1, newOnes - 1);
      res += probNoNew * getNewProb (myN - 1, myC - 1, missing, newOnes);

      probForNew[myN][myC][missing][newOnes] = res;
    }

  assert (probForNew[myN][myC][missing][newOnes] >= 0.0);
  return probForNew[myN][myC][missing][newOnes];
}


/* The recursive missing function.  */

static realT
packsForMissing (const int missing, const int curLimit)
{
  if (curLimit + 1 >= LIMIT_BOUND)
    return 0.0;

  assert (missing <= c && missing >= 0);
  if (missing == 0)
    return 0.0;

  if (neededPacks[missing][curLimit] < 0.0)
    {
      realT res = 0.0;
      for (int newOnes = 1; newOnes <= missing; ++newOnes)
        {
          res += getNewProb (n, c, missing, newOnes)
                 * packsForMissing (missing - newOnes, 0);
        }

      /* Fix for the case of no new cards.  */
      const realT noNewProb = getNewProb (n, c, missing, 0);
      res += noNewProb * packsForMissing (missing, curLimit + 1);

      neededPacks[missing][curLimit] = 1.0 + res;
    }

  assert (neededPacks[missing][curLimit] > 0.0);
  return neededPacks[missing][curLimit];
}


/* Solve a single case.  */

static realT
solve_case ()
{
  std::cin >> c >> n;
  std::fill ((realT*) neededPacks,
             ((realT*) neededPacks) + MAX_NC * LIMIT_BOUND, -1.0);
  std::fill ((realT*) probForNew,
             ((realT*) probForNew) + MAX_NC * MAX_NC * MAX_NC * MAX_NC,
             -1.0);

  return packsForMissing (c, 0);
}


/* Main routine, handling the different cases.  */

int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    printf ("Case #%d: %f\n", i, solve_case ());

  return EXIT_SUCCESS;
}
