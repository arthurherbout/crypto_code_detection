/* CodeJam solution solution in C++ by domob.  */

/* 8! = 40,320, so for the small case, just try out all permutations.  */

/* Since a return flight always visits a city where I already was, I need
   to visit cities for the first time using the outbound flights.  Thus,
   I should take precisely all allowed outbound flights to the cities
   in the order I want to visit them the first time.  This also implies that
   while *incoming* outbound flights can be at most one per city,
   *outgoing* return flights are also at most one per city.  Thus I can leave
   a city only twice, once outbound and once on the way back to where
   I came from the first time.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <vector>
#include <set>
#include <utility>
#include <stdint.h>

typedef std::pair<unsigned, unsigned> flightT;
typedef std::set<flightT> flightSet;
typedef std::set<unsigned> visitedT;

unsigned n, m;
flightSet flights;
std::vector<std::string> zips;
visitedT visited;
std::vector<unsigned> route;

/* See if a flight is possible.  */
bool
hasFlight (unsigned i, unsigned j)
{
  assert (i != j);
  if (i > j)
    std::swap (i, j);
  assert (i < j);

  return (flights.find (flightT (i, j)) != flights.end ());
}

/* Depth-first search recursion.  */
bool
solveIt (std::string& fullRes)
{
  assert (!route.empty ());
  const unsigned pos = route.back ();
  assert (visited.find (pos) != visited.end ());

  /*
  for (const auto p : route)
    std::cout << " " << p;
  std::cout << std::endl;
  */

  /* If everything is visited, we're done.  */
  if (visited.size () == n)
    return true;

  /* Try flight back.  This is only possible if we don't get back
     from the starting city, since this would end the trip and we're
     (per above) not yet done.  */
  std::string res;
  bool hasPossibility = false;
  if (route.size () > 1)
    {
      route.pop_back ();
      hasPossibility = solveIt (res);
      route.push_back (pos);
    }

  /* Try every unvisited city to which a flight exists.  */
  for (unsigned i = 0; i < n; ++i)
    if (i != pos && visited.find (i) == visited.end () && hasFlight (pos, i))
      {
        visited.insert (i);
        route.push_back (i);
        std::string cur = zips[i];
        if (solveIt (cur))
          {
            if (!hasPossibility || cur < res)
              res = cur;
            hasPossibility = true;
          }
        route.pop_back ();
        visited.erase (i);
      }

  if (!hasPossibility)
    return false;

  fullRes += res;
  return true;
}

/* Solve a single case.  */
void
solve_case ()
{
  std::cin >> n >> m;

  zips.clear ();
  zips.reserve (n);
  for (unsigned i = 0; i < n; ++i)
    {
      std::string str;
      std::cin >> str;
      assert (str.length () == 5);
      zips.push_back (str);
    }
  assert (zips.size () == n);

  flights.clear ();
  for (unsigned i = 0; i < m; ++i)
    {
      unsigned j, k;
      std::cin >> j >> k;
      assert (j < k);
      flights.insert (flightT (j - 1, k - 1));
    }
  assert (flights.size () == m);

  /* Do a depth-first search:  From each city where I am, I have the choice
     to return and to fly forward to each unvisited city.  That's all.  No
     bookkeeping required about available return flights or so.  */
  visited.clear ();
  route.clear ();
  bool hasPossibility = false;
  std::string res;
  for (unsigned i = 0; i < n; ++i)
    {
      visited.insert (i);
      route.push_back (i);
      std::string cur = zips[i];
      if (solveIt (cur))
        {
          if (!hasPossibility || cur < res)
            res = cur;
          hasPossibility = true;
        }
      route.pop_back ();
      visited.erase (i);
    }
  assert (hasPossibility);
  std::cout << res;
}

/* Main routine, handling the different cases.  */
int
main ()
{
  int n;

  scanf ("%d\n", &n);
  for (int i = 1; i <= n; ++i)
    {
      std::cout << "Case #" << i << ": ";
      solve_case ();
      std::cout << std::endl;
    }

  return EXIT_SUCCESS;
}
