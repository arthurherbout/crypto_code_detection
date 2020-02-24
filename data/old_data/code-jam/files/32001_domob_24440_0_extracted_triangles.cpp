#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdlib>
#include <stdint.h>

typedef int64_t intT;

void solveCase ()
{
  intT n, m, a;
  std::cin >> n >> m >> a;

  const intT x1(0), y1(0);
  for (intT x2(0); x2 <= n; ++x2)
    for (intT y2(0); y2 <= m; ++y2)
      for (intT x3(0); x3 <= n; ++x3)
        for (intT y3(0); y3 <= m; ++y3)
          {
            assert (x1 == 0 && y1 == 0);
            intT crossZ (x2 * y3 - x3 * y2);
            if (crossZ < 0)
              crossZ = -crossZ;

            if (crossZ == a)
              {
                std::cout << x1 << " " << y1 << " ";
                std::cout << x2 << " " << y2 << " ";
                std::cout << x3 << " " << y3;
                return;
              }
          }

  std::cout << "IMPOSSIBLE";
}

int main ()
{
  unsigned n;
  std::cin >> n;

  for (unsigned i(1); i <= n; ++i)
    {
      std::cout << "Case #" << i << ": ";
      solveCase ();
      std::cout << std::endl;
    }

  return EXIT_SUCCESS;
}
