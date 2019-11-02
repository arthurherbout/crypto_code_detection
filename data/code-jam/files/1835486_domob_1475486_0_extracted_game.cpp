/* CodeJam solution Perfect Game in C++ by domob.  */

/*

If probability to die in last level is p, its duration d and the
expected duration of all previous levels is o, then the final
expectation fulfils:

D = (o + d) * (1 - p) + (o + d + D) * p
=> D = (o + d) / (1 - p)

Thus:

D = ((d1 / (1 - p1) + d2) / (1 - p2) + ... + dN) / (1 - pN)

In the small input, always try the levels with highest probability first.

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
  realT p;
  int ind;

  inline dataT (realT a, int b)
    : p(a), ind(b)
  {}

  dataT& operator= (const dataT& o) = default;
  dataT (const dataT& o) = default;

  friend inline bool
  operator< (const dataT& a, const dataT& b)
  {
    if (a.p > b.p)
      return true;
    if (a.p < b.p)
      return false;
    return a.ind < b.ind;
  }

  friend inline bool
  operator== (const dataT& a, const dataT& b)
  {
    return (a.p == b.p) && (a.ind == b.ind);
  }

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
      assert (L == 1);
    }

  dataArr data;
  for (int i = 0; i < N; ++i)
    {
      int P;
      std::cin >> P;
      data.push_back (dataT(P / 100.0, i));
    }

  std::sort (data.begin (), data.end ());
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
