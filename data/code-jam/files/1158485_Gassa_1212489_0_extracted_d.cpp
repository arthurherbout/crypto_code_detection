#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace std;

typedef long long int64;
typedef double real;

#ifdef WIN32
#define INT64 "%I64d"
#else
#define INT64 "%lld"
#endif

const int MaxN = 10004, MaxC = 0x3F3F3F3F, NA = -1;

char buf [MaxN];
int n;

int main (void)
{
 int test, tests;
 int64 t, u, v, w;
 int i;
 scanf (" %d", &tests);
 for (test = 1; test <= tests; test++)
 {
  scanf (" %s", buf);
  n = strlen (buf);
  v = w = 0;
  for (i = 0; i < n; i++)
  {
   if (buf[i] == '0')
    v |= 1LL << (n - i - 1);
   if (buf[i] == '1')
    w |= 1LL << (n - i - 1);
  }
  u = -1;
  for (t = 1; ; t++)
  {
   u = t * t;
   if ((((~u) & v) == v) && ((u & w) == w))
    break;
  }
  for (i = 0; i < n; i++)
   if (u & (1LL << (n - i - 1)))
    buf[i] = '1';
   else
    buf[i] = '0';
  printf ("Case #%d: %s\n", test, buf);
 }
 return 0;
}
