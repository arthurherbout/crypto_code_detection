#include <iostream>
#include <vector>
#include <utility>
#include <cassert>
#include <cstdlib>
#include <stdint.h>

typedef std::pair<int64_t, int64_t> pt;
typedef std::vector<pt> ptVect;

bool isOk[3][3][3];

void initOk ()
{
  for (unsigned i(0); i!=3; ++i)
    for (unsigned j(0); j!=3; ++j)
      for (unsigned k(0); k!=3; ++k)
        isOk[i][j][k] = (((i + j + k) % 3) == 0);
}

int solveCase ()
{
  uint64_t n;
  int64_t A, B, C, D, x0, y0, M;
  std::cin >> n >> A >> B >> C >> D >> x0 >> y0 >> M;
  int64_t X, Y;

  ptVect pts;

  X = x0;
  Y = y0;
  pts.push_back (pt (X%3, Y%3));
  //std::cout << X << " " << Y << std::endl;
  for (unsigned i(1); i < n; ++i)
    {
      X = ((A) * X + B) % M;
      Y = ((C) * Y + D) % M;
      //std::cout << X << " " << Y << std::endl;
      pts.push_back (pt (X%3, Y%3));
    }
  assert (pts.size () == n);

  unsigned cnt(0);
  for (unsigned i(0); i < n; ++i)
    for (unsigned j(i+1); j < n; ++j)
      for (unsigned k(j+1); k < n; ++k)
        {
          assert (i < j && j < k);
          if (isOk[pts[i].first][pts[j].first][pts[k].first] &&
              isOk[pts[i].second][pts[j].second][pts[k].second])
            ++cnt;
        }

  return cnt;
}

int main ()
{
  int n;
  std::cin >> n;

  initOk ();

  for (int i(1); i <= n; ++i)
    std::cout << "Case #" << i << ": " << solveCase () << std::endl;

  return EXIT_SUCCESS;
}
