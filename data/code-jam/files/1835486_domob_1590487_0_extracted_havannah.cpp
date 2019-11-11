/* CodeJam solution Havannah in C++ by domob.  */

/* Basically simulate it.  Calculate and update the connected regions, and
   then after each move just recalculate those for the adjacent coordinates.
   See if one of the configurations is there afterwards.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <stdint.h>

class Coordinate
{
  public:

    typedef std::vector<Coordinate> coordArr;

    int S;
    int x, y;

    inline Coordinate (int a, int b, int c)
      : S(a), x(b), y(c)
    {}
    Coordinate (const Coordinate& o) = default;
    Coordinate& operator= (const Coordinate& o) = default;

    friend inline bool
    operator== (const Coordinate& a, const Coordinate& b)
    {
      assert (a.S == b.S);
      return a.x == b.x && a.y == b.y;
    }

    coordArr getNeighbours () const;

    inline bool
    isValid () const
    {
      if (x < 1 || y < 1)
        return false;
      if (x > 2 * S - 1 || y > 2 * S - 1)
        return false;
      if (std::abs (x - y) > S - 1)
        return false;

      return true;
    }

    enum Type
    {
      INNER, EDGE, CORNER
    };

    inline Type
    getType () const
    {
      coordArr n = getNeighbours ();
      switch (n.size ())
        {
          case 3:
            return CORNER;
          case 4:
            return EDGE;
          case 6:
            return INNER;
          default:
            assert (false);
        }
    }

    inline int
    getLinearInd () const
    {
      return 2 * S * x + y;
    }

};

Coordinate::coordArr
Coordinate::getNeighbours () const
{
  coordArr res;

  Coordinate dwn(S, x - 1, y - 1);
  if (dwn.isValid ())
    res.push_back (dwn);
  Coordinate up(S, x + 1, y + 1);
  if (up.isValid ())
    res.push_back (up);
  Coordinate lr(S, x - 1, y);
  if (lr.isValid ())
    res.push_back (lr);
  Coordinate ur(S, x, y + 1);
  if (ur.isValid ())
    res.push_back (ur);
  Coordinate ll(S, x, y - 1);
  if (ll.isValid ())
    res.push_back (ll);
  Coordinate ul(S, x + 1, y);
  if (ul.isValid ())
    res.push_back (ul);

  return res;
}

class Board
{
  public:

    int S;
    std::vector<bool> stone;
    std::vector<int> region;

    int nextRegion;

    Board (int s);

    void setStone (const Coordinate& c);

    void setConnectedRegion (const Coordinate& from, int oldR, int newR);

    void checkCond (bool& hasRing, bool& hasBridge, bool& hasFork) const;

};

Board::Board (int s)
  : S(s), stone(), region(), nextRegion(0)
{
  for (int i = 1; i < 2 * s; ++i)
    for (int j = 1; j < 2 * s; ++j)
      {
        Coordinate c(S, i, j);
        if (c.isValid ())
          {
            const int ind = c.getLinearInd ();
            stone.resize (ind + 1);
            stone[ind] = false;
            region.resize (ind + 1);
            region[ind] = nextRegion;
          }
      }

  ++nextRegion;
}

void
Board::setConnectedRegion (const Coordinate& from, int oldR, int newR)
{
  if (oldR == newR)
    return;

  const int ind = from.getLinearInd ();
  if (region[ind] != oldR)
    return;

  region[ind] = newR;
  Coordinate::coordArr ns = from.getNeighbours ();
  for (const auto& n : ns)
    setConnectedRegion (n, oldR, newR);
}

void
Board::setStone (const Coordinate& c)
{
  const int ind = c.getLinearInd ();
  assert (!stone[ind]);
  stone[ind] = true;

  Coordinate::coordArr ns;
  ns = c.getNeighbours ();

  region[ind] = nextRegion;
  ++nextRegion;

  std::set<int> filledWith;
  for (const auto& n : ns)
    {
      const int nInd = n.getLinearInd ();
      if (stone[nInd])
        setConnectedRegion (n, region[nInd], region[ind]);
      else
        {
          if (filledWith.find (region[nInd]) == filledWith.end ())
            {
              setConnectedRegion (n, region[nInd], nextRegion);
              filledWith.insert (nextRegion);
              ++nextRegion;
            }
        }
    }

  /*
  for (int i = 1; i < 2 * S; ++i)
    for (int j = 1; j < 2 * S; ++j)
      {
        Coordinate c(S, i, j);
        if (c.isValid ())
          {
            std::cout << i << " " << j << ": " << stone[c.getLinearInd ()] << ", " << region[c.getLinearInd ()] << std::endl;
          }
      }
      */
}


bool
onSameEdge (const Coordinate& c1, const Coordinate& c2)
{
  if (c1.x == 1 && c2.x == 1)
    return true;
  if (c1.y == 1 && c2.y == 1)
    return true;
  if (c1.x == 2 * c1.S - 1 && c2.x == 2 * c1.S - 1)
    return true;
  if (c1.y == 2 * c1.S - 1 && c2.y == 2 * c1.S - 1)
    return true;
  if (c1.x - c1.y == c2.x - c2.y)
    return true;

  return false;
}

void
Board::checkCond (bool& hasRing, bool& hasBridge, bool& hasFork) const
{
  Coordinate::coordArr edges;
  Coordinate::coordArr corners;
  Coordinate::coordArr inners;
  std::set<int> edgeRegions;

  for (int i = 1; i < 2 * S; ++i)
    for (int j = 1; j < 2 * S; ++j)
      {
        Coordinate c(S, i, j);
        if (!c.isValid ())
          continue;

        Coordinate::Type t = c.getType ();
        switch (t)
          {
            case Coordinate::EDGE:
              edges.push_back (c);
              if (!stone[c.getLinearInd ()])
                edgeRegions.insert (region[c.getLinearInd ()]);
              break;
            case Coordinate::CORNER:
              corners.push_back (c);
              if (!stone[c.getLinearInd ()])
                edgeRegions.insert (region[c.getLinearInd ()]);
              break;
            case Coordinate::INNER:
              inners.push_back (c);
              break;
            default:
              assert (false);
          }
      }

  /*
  std::cout << "Edge regions: ";
  for (const auto& i : edgeRegions)
    std::cout << " " << i;
  std::cout << std::endl;
  */

  hasRing = false;
  for (const auto& c : inners)
    {
      const int ind = c.getLinearInd ();
      if (stone[ind])
        continue;
      if (edgeRegions.find (region[ind]) == edgeRegions.end ())
        {
          hasRing = true;
          break;
        }
    }

  hasFork = false;
  for (const auto& c1 : edges)
    {
      const int ind1 = c1.getLinearInd ();
      if (!stone[ind1])
        continue;

      for (const auto& c2 : edges)
        {
          const int ind2 = c2.getLinearInd ();
          if (!stone[ind2])
            continue;

          if (onSameEdge (c1, c2))
            continue;

          for (const auto& c3 : edges)
            {
              const int ind3 = c3.getLinearInd ();
              if (!stone[ind3])
                continue;

              if (onSameEdge (c2, c3) || onSameEdge (c1, c3))
                continue;

              if (region[ind1] == region[ind2] && region[ind1] == region[ind3])
                {
                  hasFork = true;
                  goto forkEnd;
                }
            }
        }
    }
forkEnd:;

  hasBridge = false;
  for (const auto& c1 : corners)
    {
      const int ind1 = c1.getLinearInd ();
      if (!stone[ind1])
        continue;

      for (const auto& c2 : corners)
        {
          const int ind2 = c2.getLinearInd ();
          if (!stone[ind2])
            continue;

          if (c1 == c2)
            continue;

          if (region[ind1] == region[ind2])
            {
              hasBridge = true;
              goto bridgeEnd;
            }
        }
    }
bridgeEnd:;
}


/* Solve a single case.  */

void
solve_case ()
{
  int S, M;
  std::cin >> S >> M;

  Board b(S);
  bool done = false;
  for (int i = 1; i <= M; ++i)
    {
      int x, y;
      std::cin >> x >> y;
      Coordinate c(S, x, y);
      assert (c.isValid ());
      b.setStone (c);

      if (!done)
        {
          bool hasRing, hasBridge, hasFork;
          b.checkCond (hasRing, hasBridge, hasFork);

          if (hasBridge)
            {
              std::cout << "bridge";
              done = true;
              if (hasFork)
                {
                  std::cout << "-fork";
                  if (hasRing)
                    std::cout << "-ring";
                }
              else if (hasRing)
                std::cout << "-ring";
            }
          else if (hasFork)
            {
              std::cout << "fork";
              done = true;
              if (hasRing)
                std::cout << "-ring";
            }
          else if (hasRing)
            {
              std::cout << "ring";
              done = true;
            }

          if (done)
            std::cout << " in move " << i;
        }
    }

  if (!done)
    std::cout << "none";
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
