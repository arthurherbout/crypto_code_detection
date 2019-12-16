/* CodeJam solution descending C++ by domob.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <utility>
#include <vector>
#include <set>
#include <stdint.h>

typedef std::pair<unsigned, unsigned> coordT;
typedef std::vector<coordT> coordArr;
typedef std::set<coordT> coordSet;

#define MAX_SIZE 10

unsigned R, C;
char map[MAX_SIZE][MAX_SIZE];


/* Step in some direction with all coordinates in a set.  */

coordSet
performStep (const coordSet& original, int dirRow, int dirCol)
{
  coordSet res;
  for (const auto& c : original)
    {
      const unsigned ni = c.first + dirRow;
      const unsigned nj = c.second + dirCol;
      
      if (map[ni][nj] != '#')
        res.insert (coordT(ni, nj));
      else
        res.insert (c);
    }

  return res;
}


/* For a given cave, find possible original positions.  */

coordSet
findOriginals (const coordT& cave)
{
  coordSet S;
  coordArr todo;

  S.insert (cave);
  todo.push_back (cave);

  while (!todo.empty ())
    {
      const coordT cur = todo.back ();
      todo.pop_back ();

      coordArr trials;
      trials.push_back (coordT(cur.first - 1, cur.second)); // Above.
      trials.push_back (coordT(cur.first, cur.second + 1)); // Right.
      trials.push_back (coordT(cur.first, cur.second - 1)); // Left.

      for (const auto& t : trials)
        {
          if (S.find (t) != S.end ())
            continue;

          if (t.first > R - 1 || t.second > C - 1)
            continue;

          if (map[t.first][t.second] == '#')
            continue;

          S.insert (t);
          todo.push_back (t);
        }
    }

  return S;
}


/* Check if a cave is lucky.  Simply by trying all possible plans.  */

bool
isLucky (const coordT cave, const coordSet& S)
{
  /* See if it is possible at all.  */
  coordSet possible = findOriginals (cave);
  for (const auto& i : S)
    if (possible.find (i) == possible.end ())
      return false;

  /*
  for (const auto& i : S)
    std::cout << i.first << " " << i.second << "; ";
  std::cout << std::endl;
  */

  /* Else, try stepping up to 8 times right, up to 8 times left and
     then optionally down.  */
  for (int d = -1; d <= 1; d += 2)
    {
      coordSet curS = S;
      for (unsigned lefts = 0; lefts <= MAX_SIZE - 2; ++lefts)
        {
          //std::cout << "a" << std::endl;
          if (curS.empty ())
            break;
          if (curS.size () == 1 && *curS.begin () == cave)
            {
            //std::cout << 1 << std::endl;
            return true;
            }

          coordSet nextS = curS;
          for (unsigned rights = 0; rights <= MAX_SIZE - 2; ++rights)
            {
          //std::cout << "a" << std::endl;
              if (nextS.empty ())
                break;
              if (nextS.size () == 1 && *nextS.begin () == cave)
                {
                //std::cout << 2 << std::endl;
                return true;
                }

              coordSet dwn = performStep (nextS, 1, 0);
              if (dwn != nextS && isLucky (cave, dwn))
                {
                //std::cout << 3 << std::endl;
                return true;
                }

              nextS = performStep (nextS, 0, d);
            }

          curS = performStep (curS, 0, -d);
        }
    }

  return false;
}


/* Solve a single case.  */

void
solve_case ()
{
  std::cin >> R >> C;

  coordArr caves;

  assert (R <= MAX_SIZE && C <= MAX_SIZE);
  for (unsigned i = 0; i < R; ++i)
    for (unsigned j = 0; j < C; ++j)
      {
        char c;
        do
          c = std::cin.get ();
        while (c == '\n');

        assert (c == '.' || c == '#' || (c >= '0' && c <= '9'));
        map[i][j] = c;

        if (c >= '0' && c <= '9')
          {
            const unsigned d = (c - '0');
            if (caves.size () < d + 1)
              caves.resize (d + 1);
            caves[d].first = i;
            caves[d].second = j;
          }
      }

  for (unsigned c = 0; c < caves.size (); ++c)
    {
      coordSet S = findOriginals (caves[c]);
      const bool lucky = isLucky (caves[c], S);
      printf ("%d: %d %s\n", c, S.size (), (lucky ? "Lucky" : "Unlucky"));
    }
}


/* Main routine, handling the different cases.  */

int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    {
      printf ("Case #%d:\n", i);
      solve_case ();
    }

  return EXIT_SUCCESS;
}
