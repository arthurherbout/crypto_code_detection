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

const int LET = 26, MaxN = LET << 1, MaxL = 5007, MaxC = 0x3F3F3F3F, NA = -1;

bool a [LET] [LET];
bool b [LET];
int c [MaxN];
int d [MaxN];
int e [MaxN];
char s [MaxL];
int k, n, p, res;

void recur (int i)
{
 if (c[i])
 {
  assert (c[i] == p);
  return;
 }
 c[i] = p;
 eprintf ("recur  %d %c %d\n", p, (i % LET) + 'a', i / LET);

 int j;
 for (j = 0; j < MaxN; j++)
  if (a[i % LET][j % LET] || a[j % LET][i % LET])
   recur (j);
}

int main (void)
{
 int test, tests;
 int i, j;
 int cur, temp;

 for (i = 0; i < LET; i++)
  b[i] = (string ("oieastbg").find (i + 'a') != string::npos);

 scanf (" %d ", &tests);
 for (test = 1; test <= tests; test++)
 {
  scanf (" %d", &k);
  scanf (" %s", s);

  n = strlen (s);
  memset (a, 0, sizeof (a));
  for (i = 1; i < n; i++)
   a[s[i - 1] - 'a'][s[i] - 'a'] = true;

  memset (d, 0, sizeof (d));
  memset (e, 0, sizeof (e));
  for (i = 0; i < LET; i++)
   for (j = 0; j < LET; j++)
    if (a[i][j])
    {
     d[i]++;
     e[j]++;
     if (b[i])
     {
      d[i + LET]++;
      e[j]++;
     }
     if (b[j])
     {
      d[i]++;
      e[j + LET]++;
     }
     if (b[i] && b[j])
     {
      d[i + LET]++;
      e[j + LET]++;
     }
    }

  res = 0;
  memset (c, 0, sizeof (c));
  p = 0;
  for (i = 0; i < MaxN; i++)
   if ((d[i] || e[i]) && !c[i])
   {
    p++;
    recur (i);

    cur = 0;
    for (j = 0; j < MaxN; j++)
     if (c[j] == p)
      cur += d[j];
    cur++;

    temp = 0;
    for (j = 0; j < MaxN; j++)
     if (c[j] == p)
      temp += max (0, d[j] - e[j]);
    temp = max (0, temp - 1);
//    temp >>= 1;

    eprintf ("cur = %d, temp = %d\n", cur, temp);
    res += cur + temp;
   }
  assert (p == 1);

  eprintf ("!\n");
  printf ("Case #%d: %d\n", test, res);
 }

 return 0;
}
