#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <queue>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

typedef long long int64;
typedef unsigned long long uint64;
typedef long double real;

#ifdef WIN32
#define INT64 "%I64d"
#define UINT64 "%I64u"
#else
#define INT64 "%lld"
#define UINT64 "%llu"
#endif

#ifdef DEBUG
#define eprintf(...) fprintf (stderr, __VA_ARGS__)
#else
#define eprintf(...) assert (true)
#endif

const int MaxN = 2000009, MaxK = 205, MaxC = 0x3F3F3F3F, NA = -1;

pair <int64, int64> a [MaxN];
int64 g [MaxN];
int h [MaxN];
int d [MaxK];
int64 e [MaxK];
int n;
int64 f, m;

int main (void)
{
 int test, tests;
 int i, j, k;
 int64 res;

 scanf (" %d ", &tests);
 for (test = 1; test <= tests; test++)
 {
  scanf (" " INT64 " " INT64 " %d", &m, &f, &n);
  for (i = 0; i < n; i++)
   scanf (" " INT64 " " INT64, &a[i].first, &a[i].second);
  sort (a, a + n);

  memset (g, MaxC, sizeof (g));
  k = 1;
  g[0] = 0;
  for (i = 0; i < n; i++)
  {
   for ( ; k <= a[i].second + 1; k++)
    g[k] = min ((int64) g[k], (int64) g[k - 1] + a[i].first);
   k--;
   d[i] = k;
   e[i] = g[k];
   eprintf ("%d %d\n", d[i], (int) e[i]);
   k++;
  }

  memset (h, 0, sizeof (h));
  for (j = 0; j <= m; j++)
  {
   for (i = 0; i < n; i++)
    if (j - f - e[i] >= 0)
     h[j] = max ((int64) h[j], (int64) h[j - f - e[i]] + d[i]);
  }

  for (j = 1; j <= m; j++)
   h[j] = max (h[j], h[j - 1]);
  res = h[m];
  for (j = 0; j <= m; j++)
   res = max ((int64) res, (int64) h[j]);

/*
  for (j = 0; j <= m; j++)
   printf (INT64 " ", g[j]);
  printf ("\n");
  for (j = 0; j <= m; j++)
   printf ("%d ", h[j]);
  printf ("\n");
*/

  for (j = 1; j <= m; j++)
   if (m - f - g[j] >= 0)
    res = max ((int64) res, (int64) h[m - f - g[j]] + j);

  printf ("Case #%d: " INT64 "\n", test, res);
 }

 return 0;
}
