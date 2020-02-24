/* CodeJam solution trains in C++ by domob.  */

/* Proud of Conchita Wurst & her global message of tolerance!  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <stdint.h>

typedef unsigned long long intT;
const intT MOD = 1000000007ull;

typedef std::vector<std::string> strArr;
typedef std::set<char> charSet;
typedef std::set<int> intSet;

unsigned n;
charSet usedChars;
intSet usedStrings;
strArr strings;

/* Recursive solution method.  */
intT
solveIt (const char lastChr)
{
  intT res = 0;
  bool foundString = false;
  for (unsigned i = 0; i < n; ++i)
    if (usedStrings.find (i) == usedStrings.end ())
      {
        foundString = true;

        const charSet oldUsedChars = usedChars;
        char curLastChr = lastChr;
        for (char c : strings[i])
          {
            if (c == curLastChr)
              continue;

            if (usedChars.find (c) != usedChars.end ())
              goto continueOuter;
            usedChars.insert (c);
            curLastChr = c;
          }

        usedStrings.insert (i);
        res += solveIt (curLastChr);
        usedStrings.erase (i);

continueOuter:;
        usedChars = oldUsedChars;
      }

  /* If no more strings are available, we're simply done.  */
  if (!foundString)
    {
      assert (res == 0);
      return 1;
    }

  return res;
}

/* Solve a single case.  */
void
conchita ()
{
  std::cin >> n;

  strings.resize (n);
  assert (strings.size () == n);
  for (unsigned i = 0; i < n; ++i)
    std::cin >> strings[i];

  /* For the small test case, try all permutations.  This should be possible
     in time and allows to get a (hopefully) correct program first.  This
     can then be used to validate optimised large solutions.  */
  usedChars.clear ();
  usedStrings.clear ();
  std::cout << solveIt ('0');
}

/* Main routine, handling the different cases.  */
int
main ()
{
  int n;

  std::cin >> n;
  for (int i = 1; i <= n; ++i)
    {
      std::cout << "Case #" << i << ": ";
      conchita ();
      std::cout << std::endl;
    }

  return EXIT_SUCCESS;
}
