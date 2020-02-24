#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdlib>
#include <stdint.h>

typedef int32_t intT;

#define MAX_WH 101

intT w, h;
intT moves[MAX_WH][MAX_WH];

intT getMoves (intT x, intT y)
{
  assert (x >= 1 && y >= 1);
  assert (x <= w && y <= h);

  if (moves[x][y] == -1)
    {
      moves[x][y] = 0;

      if (x >= 3 && y >= 2)
        moves[x][y] += getMoves (x - 2, y - 1);
      if (y >= 3 && x >= 2)
        moves[x][y] += getMoves (x - 1, y - 2);

      moves[x][y] %= 10007;
    }

  assert (moves[x][y] != -1);
  return moves[x][y];
}

intT solveCase ()
{
  intT r;

  std::cin >> h >> w >> r;

  for (intT x (0); x <= w; ++x)
    for (intT y(0); y <= h; ++y)
      moves[x][y] = -1;

  moves[1][1] = 1;
  for (intT i (0); i != r; ++i)
    {
      intT row, col;
      std::cin >> row >> col;
      moves[col][row] = 0;
    }

  return getMoves (w, h);
}

int main ()
{
  unsigned n;
  std::cin >> n;

  for (unsigned i(1); i <= n; ++i)
    {
      std::cout << "Case #" << i << ": ";
      std::cout << solveCase ();
      std::cout << std::endl;
      std::cerr << "Solved case " << i << std::endl;
    }

  return EXIT_SUCCESS;
}
