/* CodeJam solution Perfect Game in C++ by domob.  */

/*

If probability to die in last level is p, its duration d and the
expected duration of all previous levels is o, then the final
expectation fulfils:

D = (o + d) * (1 - p) + (o + d + D) * p
=> D = (o + d) / (1 - p)

Thus:

D = ((d1 / (1 - p1) + d2) / (1 - p2) + ... + dN) / (1 - pN)

E.g.,

D = d1 / (s1 s2 s3) + d2 / (s2 s3) + d3 / s3,
with si = 1 - pi.

Or: D = d1 / (s1 s2) + d2 / s2.

If i and i+1 are exchanged, the difference will be:

D_before = ... + di / (si si+1 si+2 ...) + di+1 / (si+1 si+2 ...) + ...
D_after = ... + di / (si si+2 ...) + di+1 / (si si+1 si+2 ...) + ...

=> diff = D_after - D_before = (di si+1 - di+1 si + di+1 - di) / something > 0

In the small input, always try the levels with highest probability first.
For the large input, apply bubble sort with condition above.

*/

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <stdint.h>

typedef double realT;

struct dataT
{
  realT d;
  realT p;
  realT s;
  int ind;

  inline dataT (realT l, realT a, int b)
    : d(l), p(a), s(1.0 - a), ind(b)
  {}

  dataT& operator= (const dataT& o) = default;
  dataT (const dataT& o) = default;

};

typedef std::vector<dataT> dataArr;


/* Solve a single case.  */

void
solve_case ()
{
  int N;
  std::cin >> N;

  std::vector<int> Ls;
  for (int i = 0; i < N; ++i)
    {
      int L;
      std::cin >> L;
      Ls.push_back (L);
    }

  dataArr data;
  for (int i = 0; i < N; ++i)
    {
      int P;
      std::cin >> P;
      data.push_back (dataT(Ls[i], P / 100.0, i));
    }

  bool foundSomething;
  do
    {
      foundSomething = false;
      for (int i = 0; i < N - 1; ++i)
        {
          dataT cur = data[i];
          dataT next = data[i + 1];

          const realT val = cur.d * next.s - next.d * cur.s + next.d - cur.d;
          if (val < 0)
            {
              foundSomething = true;
              data[i] = next;
              data[i + 1] = cur;
              break;
            }
        }
    }
  while (foundSomething);

  for (const auto& i : data)
    std::cout << " " << i.ind;
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
