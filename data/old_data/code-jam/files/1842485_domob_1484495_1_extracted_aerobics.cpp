/* CodeJam solution aerobics in C++ by domob.  */

/* Simply try Monte-Carlo....  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <utility>
#include <stdint.h>

typedef double realT;
inline double
getRandom (int upper)
{
  return upper * (static_cast<realT> (std::rand ()) / RAND_MAX);
}

typedef std::pair<int, int> radiusEl;
typedef std::vector<radiusEl> radiusArr;
typedef std::vector<realT> coordArr;

inline bool
lessThan (const radiusEl& a, const radiusEl& b)
{
  return a.first > b.first;
}


/* Solve a single case.  */

void
solve_case ()
{
  int N, W, L;
  std::cin >> N >> W >> L;
  radiusArr ri;
  for (int i = 0; i < N; ++i)
    {
      int r;
      std::cin >> r;
      ri.push_back (radiusEl(r, i));
    }
  std::sort (ri.begin (), ri.end (), &lessThan);

  coordArr x, y;
  int tries = 0;
  while (x.size () < N)
    {
      if (tries > 1000 && !x.empty ())
        {
          x.pop_back ();
          y.pop_back ();
          tries = 0;
          continue;
        }

      const realT nx = getRandom (W);
      const realT ny = getRandom (L);
      ++tries;

      for (int i = 0; i < x.size (); ++i)
        {
          const realT dx = nx - x[i];
          const realT dy = ny - y[i];
          const realT r = ri[i].first + ri[x.size ()].first;
          if (dx * dx + dy * dy < r * r)
            goto freshTry;
        }

      x.push_back (nx);
      y.push_back (ny);
      ++tries;

freshTry:;
    }

  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      if (i == ri[j].second)
        printf (" %f %f", x[j], y[j]);
}


/* Main routine, handling the different cases.  */

int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    {
      printf ("Case #%d:", i);
      solve_case ();
      printf ("\n");
    }

  return EXIT_SUCCESS;
}
