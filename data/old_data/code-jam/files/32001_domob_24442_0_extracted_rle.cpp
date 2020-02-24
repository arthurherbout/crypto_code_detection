#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <utility>
#include <cassert>
#include <cstdlib>
#include <stdint.h>

#define MAX_K 16
#define MAX_SLEN 50000

void permutate (const char* in, char* out, unsigned k, unsigned* perm)
{
  for (unsigned i(0); i != k; ++i)
    out[i] = in[perm[i]];
}

int solveCase ()
{
  unsigned k;
  std::string s;
  std::cin >> k >> s;
  assert (s.size () % k == 0);

  unsigned perm[MAX_K];
  for (unsigned i(0); i != k; ++i)
    perm[i] = i;

  const char* origStr(s.c_str());
  char permStr[MAX_SLEN+1];
  permStr[s.size ()] = 0;

  unsigned res(s.size ());
  unsigned cnt(0);
  do
    {
      for (unsigned start(0); start < s.size (); start += k)
        permutate(origStr + start, permStr + start, k, perm);

      char lastChar(permStr[0]);
      unsigned rleLen(1);
      for (unsigned i(1); i < s.size (); ++i)
        if (permStr[i] != lastChar)
          {
            ++rleLen;
            lastChar = permStr[i];
          }

      if (rleLen < res)
        res = rleLen;

      ++cnt;
    }
  while (std::next_permutation (perm, perm + k));
  //std::cerr << cnt << std::endl;

  return res;
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
