/* CodeJam solution trains in C++ by domob.  */

/* Proud of Conchita Wurst & her global message of tolerance!  */

//#define NDEBUG

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <deque>
#include <set>
#include <stdint.h>

typedef unsigned long long intT;
const intT MOD = 1000000007ull;

typedef std::vector<std::string> strArr;
typedef std::deque<std::string> strQueue;

unsigned n;
strArr strings;

/* Cached factorial modulo MOD.  */
intT factorials[101];
intT
factorial (unsigned n)
{
  if (n == 0 || n == 1)
    return 1;

  assert (n < 101);
  if (factorials[n] != 0)
    return factorials[n];

  const intT res = (n * factorial (n - 1)) % MOD;
  assert (res < MOD);
  factorials[n] = res;

  return res;
}

/* Characters "in the middle" of strings are only relevant if they
   appear in some other strings as well.  If that's the case, the
   situation is impossible anyway.  Thus remove all intermediate characters
   and see if the "impossible" case happens.  */
bool
removeMiddles ()
{
  /* In a first step, remove duplicates.  */
  for (auto& cur : strings)
    {
      std::ostringstream out;
      char last = '0';
      for (auto c : cur)
        if (last != c)
          {
            out << c;
            last = c;
          }

      cur = out.str ();
    }

  /* Now, really remove middles.  */
  for (unsigned i = 0; i < n; ++i)
    if (strings[i].size () > 1)
      {
        std::ostringstream out;
        out << strings[i].front () << strings[i].back ();

        /* Take care that also the first character may not repeat
           later on in this string!  */
        for (unsigned ci = 0; ci < strings[i].size () - 1; ++ci)
          {
            const char c = strings[i][ci];
            for (unsigned cj = ci + 1; cj < strings[i].size (); ++cj)
              if (strings[i][cj] == c)
                return false;
            if (ci > 0)
              for (unsigned j = i + 1; j < n; ++j)
                if (strings[j].find (c) != std::string::npos)
                  return false;
          }

        strings[i] = out.str ();
        assert (strings[i].size () == 2);
        assert (strings[i][0] != strings[i][1]);
      }

  return true;
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

  if (!removeMiddles ())
    goto fail;

  /* Now, glue together parts that form a "domino sequence".  For those,
     the only remaining degrees of freedom are permutations of single
     character (reduced) strings.  In the end, we can also permute them.

     Take care that no characters are used in more than one sequence
     at the same time!  */

  {
    std::vector<strQueue> parts;
    std::set<char> usedChars;
    std::vector<bool> usedStrings(n, false);
    unsigned remaining = n;
    strQueue cur;

    /*
    for (auto s : strings)
      std::cout << s << std::endl;
    */

    while (remaining > 0)
      {
        //std::cout << remaining << std::endl;
        if (cur.empty ())
          {
            for (unsigned i = 0; i < n; ++i)
              if (!usedStrings[i])
                {
                  cur.push_back (strings[i]);
                  usedStrings[i] = true;
                  --remaining;

                  /* Check that the characters are not yet used.  */
                  //std::cout << "wanting: " << strings[i] << std::endl;
                  for (auto c : strings[i])
                    {
                      if (usedChars.find (c) != usedChars.end ())
                        goto fail;
                      usedChars.insert (c);
                    }

                  //std::cout << "using: " << strings[i] << std::endl;
                  break;
                }
          }

        assert (!cur.empty ());
        /* Now we have a non-empty queue to attach strings to.  */
        bool attached = false;

        /* Try to attach as many single character strings as possible
           to both ends.  */
        for (unsigned i = 0; i < n; ++i)
          if (!usedStrings[i] && strings[i].size () == 1)
            {
              if (strings[i][0] == cur.front ().front ())
                {
                  cur.push_front (strings[i]);
                  usedStrings[i] = true;
                  --remaining;
                  assert (usedChars.find (strings[i][0]) != usedChars.end ());
                  attached = true;
                  //std::cout << "attach front: " << strings[i] << std::endl;
                }
              else if (strings[i][0] == cur.back ().back ())
                {
                  cur.push_back (strings[i]);
                  usedStrings[i] = true;
                  --remaining;
                  assert (usedChars.find (strings[i][0]) != usedChars.end ());
                  attached = true;
                  //std::cout << "attach back: " << strings[i] << std::endl;
                }
            }

        /* Possibly also attach a double-char string to each end.  */
        bool haveFront = false;
        bool haveBack = false;
        for (unsigned i = 0; i < n; ++i)
          if (!usedStrings[i] && strings[i].size () == 2)
            {
              if (!haveFront && strings[i][1] == cur.front ().front ())
                {
                  cur.push_front (strings[i]);
                  usedStrings[i] = true;
                  --remaining;
                  assert (usedChars.find (strings[i][1]) != usedChars.end ());
                  if (usedChars.find (strings[i][0]) != usedChars.end ())
                    goto fail;
                  usedChars.insert (strings[i][0]);
                  attached = true;
                  haveFront = true;
                  //std::cout << "attach front: " << strings[i] << std::endl;
                }
              else if (!haveBack && strings[i][0] == cur.back ().back ())
                {
                  cur.push_back (strings[i]);
                  usedStrings[i] = true;
                  --remaining;
                  assert (usedChars.find (strings[i][0]) != usedChars.end ());
                  if (usedChars.find (strings[i][1]) != usedChars.end ())
                    goto fail;
                  usedChars.insert (strings[i][1]);
                  attached = true;
                  haveBack = true;
                  //std::cout << "attach back: " << strings[i] << std::endl;
                }
            }

        /* If we didn't attach anything, finalise current queue.  */
        if (!attached)
          {
            parts.push_back (cur);
            cur.clear ();
          }
      }

    if (!cur.empty ())
      parts.push_back (cur);

    /* Now, calculate the result.  */
    intT res = factorial (parts.size ());
    for (const auto& p : parts)
      {
        char lastC = '0';
        unsigned singleCnt = 0;
        for (const auto& s : p)
          {
            if (s.size () == 1)
              {
                if (s[0] != lastC)
                  {
                    res *= factorial (singleCnt);
                    res %= MOD;
                    lastC = s[0];
                    singleCnt = 1;
                  }
                else
                  ++singleCnt;
              }
            else if (singleCnt > 0)
              {
                res *= factorial (singleCnt);
                res %= MOD;
                lastC = '0';
                singleCnt = 0;
              }
          }

        if (singleCnt > 0)
          {
            res *= factorial (singleCnt);
            res %= MOD;
            lastC = '0';
            singleCnt = 0;
          }
      }

    assert (res < MOD);
    std::cout << res;
    return;
  }

  assert (false);
fail:
  std::cout << 0;
}

/* Main routine, handling the different cases.  */
int
main ()
{
  for (auto& f : factorials)
    f = 0;

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
