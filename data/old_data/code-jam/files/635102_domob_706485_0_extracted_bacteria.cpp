/* CodeJam solution bacteria in C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>

/* This is SMALL ONLY!  */
static const int MAX_COORD = 101;

bool isThere[MAX_COORD][MAX_COORD];


/* Do a step.  */

bool
doStep ()
{
  bool res = false;

  bool newThere[MAX_COORD][MAX_COORD];

  for (int x = 1; x < MAX_COORD; ++x)
    for (int y = 1; y < MAX_COORD; ++y)
      {
        const bool hasNorth = isThere[x][y - 1];
        const bool hasWest = isThere[x - 1][y];

        if (isThere[x][y])
          newThere[x][y] = hasNorth || hasWest;
        else
          newThere[x][y] = hasNorth && hasWest;
      }

  for (int x = 1; x < MAX_COORD; ++x)
    for (int y = 1; y < MAX_COORD; ++y)
      {
        isThere[x][y] = newThere[x][y];

        if (isThere[x][y])
          res = true;
      }

  return res;
}


/* Solve a single case.  */

void
solve_case ()
{
  int r;
  scanf ("%d", &r);

  for (int i = 0; i < MAX_COORD; ++i)
    for (int j = 0; j < MAX_COORD; ++j)
      isThere[i][j] = false;

  for (int i = 0; i < r; ++i)
    {
      int x1, x2, y1, y2;
      scanf ("%d %d %d %d", &x1, &y1, &x2, &y2);

      for (int x = x1; x <= x2; ++x)
        for (int y = y1; y <= y2; ++y)
          isThere[x][y] = true;
    }

  int steps = 1;
  while (doStep ())
    ++steps;

  printf ("%d", steps);
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
