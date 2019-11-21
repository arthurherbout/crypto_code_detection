/* CodeJam solution trie sharding in C++ by domob.  */

/* Trying out all possible assignments for small is just 8^4 = 4096,
   so totally possible.  Do it.  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <stdint.h>
#include <vector>
#include <set>

static const unsigned MOD = 1000000007;

typedef std::vector<std::string> strArr;

/* Construct trie and get number of nodes required.  */
static unsigned
getNodes (const strArr& strings)
{
  std::set<std::string> strSet;
  strSet.insert ("");

  for (const auto& s : strings)
    for (unsigned i = 0; i < s.size (); ++i)
      {
        std::string prefix(s.begin (), s.begin () + i + 1);
        strSet.insert (prefix);
      }

  return strSet.size ();
}

/* Recursive trying out.  */
static void
doIt (const strArr& strings, std::vector<strArr>& servers,
      unsigned pos, unsigned& res, unsigned& resWays)
{
  assert (pos <= strings.size ());
  if (pos == strings.size ())
    {
      unsigned cur = 0;
      for (const auto& arr : servers)
        {
          if (arr.empty ())
            return;

          cur += getNodes (arr);
        }

      if (cur > res)
        {
          res = cur;
          resWays = 1;

          /*
          std::cout << "\n\n\n";
          for (const auto& arr : servers)
            {
              for (const auto& s : arr)
                std::cout << " " << s;
              std::cout << std::endl;
            }
          */
        }
      else if (cur == res)
        resWays = (resWays + 1) % MOD;

      return;
    }

  assert (pos < strings.size ());
  for (unsigned i = 0; i < servers.size (); ++i)
    {
      servers[i].push_back (strings[pos]);

      doIt (strings, servers, pos + 1, res, resWays);

      assert (servers[i].back () == strings[pos]);
      servers[i].pop_back ();
    }
}

/* Solve a single case.  */
static void
solve_case ()
{
  unsigned m, n;
  std::cin >> m >> n;

  strArr strings(m);
  for (unsigned i = 0; i < m; ++i)
    std::cin >> strings[i];
  assert (strings.size () == m);

  unsigned res = 0;
  unsigned resWays;
  std::vector<strArr> servers(n);
  doIt (strings, servers, 0, res, resWays);

  assert (resWays < MOD);
  std::cout << res << " " << resWays;
}

/* Main routine, handling the different cases.  */
int
main ()
{
  unsigned n;

  std::cin >> n;
  //scanf ("%u\n", &n);
  for (unsigned i = 1; i <= n; ++i)
    {
      std::cout << "Case #" << i << ": ";
      //printf ("Case #%u: ", i);

      solve_case ();

      std::cout << std::endl;
      //printf ("\n");
    }

  return EXIT_SUCCESS;
}
