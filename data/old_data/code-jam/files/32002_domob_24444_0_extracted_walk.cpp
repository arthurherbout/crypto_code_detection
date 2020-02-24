#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdlib>
#include <stdint.h>

typedef int64_t intT;
typedef float floatT;

typedef std::pair<intT, intT> point;
typedef std::vector<point> walk;

enum direction
{
  NORTH, SOUTH, EAST, WEST
};

int solveCase ()
{
  unsigned l;
  std::cin >> l;

  intT minX (0), maxX (0), minY (0), maxY (0);

  walk w;
  point cur (0, 0);
  w.push_back (cur);
  direction dir (NORTH);
  for (unsigned i (0); i != l; ++i)
    {
      std::string s;
      unsigned t;
      std::cin >> s >> t;

      for (unsigned j (0); j != t; ++j)
        for (std::string::const_iterator k (s.begin ()); k != s.end (); ++k)
          switch (*k)
            {
              case 'F':
                switch (dir)
                  {
                    case NORTH:
                      --cur.second;
                      break;
                    case SOUTH:
                      ++cur.second;
                      break;
                    case EAST:
                      ++cur.first;
                      break;
                    case WEST:
                      --cur.first;
                      break;
                    default:
                      assert (false);
                  }
                w.push_back (cur);
                if (cur.first < minX) minX = cur.first;
                if (cur.first > maxX) maxX = cur.first;
                if (cur.second < minY) minY = cur.second;
                if (cur.second > maxY) maxY = cur.second;
                break;

              case 'L':
                switch (dir)
                  {
                    case NORTH:
                      dir = WEST;
                      break;
                    case SOUTH:
                      dir = EAST;
                      break;
                    case EAST:
                      dir = NORTH;
                      break;
                    case WEST:
                      dir = SOUTH;
                      break;
                    default:
                      assert (false);
                  }
                break;

              case 'R':
                switch (dir)
                  {
                    case NORTH:
                      dir = EAST;
                      break;
                    case SOUTH:
                      dir = WEST;
                      break;
                    case EAST:
                      dir = SOUTH;
                      break;
                    case WEST:
                      dir = NORTH;
                      break;
                    default:
                      assert(false);
                  }
                break;

              default:
                assert (false);
            }
    }
  assert (w.back ().first == 0);
  assert (w.back ().second == 0);
  w.push_back (w.front ());

  intT area (0);
  for (floatT x (minX + .5); x < maxX; ++x)
    for (floatT y (minY + .5); y < maxY; ++y)
      {
        intT foundN (0), foundS (0), foundW (0), foundE (0);

        for (walk::const_iterator i (w.begin()); i + 1 != w.end (); ++i)
          {
            const point& p1 (*i);
            const point& p2 (*(i + 1));
            if (p1.first == p2.first &&
                ((p1.second < y && p2.second > y) ||
                 (p1.second > y && p2.second < y)))
              {
                if (p1.first < x)
                  ++foundW;
                else
                  ++foundE;
              }

            if (p1.second == p2.second &&
                ((p1.first < x && p2.first > x) ||
                 (p1.first > x && p2.first < x)))
              {
                if (p1.second < y)
                  ++foundN;
                else
                  ++foundS;
              }
          }

        bool inside (false);
        if (foundN%2 == 1 && foundS%2 == 1 && foundE%2 == 1 && foundW%2 == 1)
          inside = true;

        if (!inside)
          if ((foundN && foundS) || (foundW && foundE))
            {
              ++area;
            }
      }

  return area;
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
    }

  return EXIT_SUCCESS;
}
