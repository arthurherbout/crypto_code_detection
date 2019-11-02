#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <queue>
#include <set>
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

const int MaxN = 64, MaxK = 12, MaxC = 0x3F3F3F3F, NA = -1;

char a [MaxN] [MaxN];
bool b [MaxN] [MaxN];
int vr [MaxK], vc [MaxK];
set <pair <int, int> > ls, rs;
int r, c, n, lr;

int recur (int vr, int vc)
{
 if (b[vr][vc])
  return 0;
 b[vr][vc] = true;
 lr = min (lr, vr);

 int res;
 res = 1;

 if (a[vr - 1][vc] == '.')
  res += recur (vr - 1, vc);

 if (a[vr][vc - 1] == '.')
  res += recur (vr, vc - 1);
 else
  ls.insert (make_pair (vr, vc));

 if (a[vr][vc + 1] == '.')
  res += recur (vr, vc + 1);
 else
  rs.insert (make_pair (vr, vc));

 return res;
}

set <pair <int, int> > move (set <pair <int, int> > s, int dr, int dc)
{
 set <pair <int, int> > res;
 set <pair <int, int> >::iterator it;

 for (it = s.begin (); it != s.end (); it++)
 {
  int cr = it -> first, cc = it -> second;
  if (a[cr + dr][cc + dc] == '.')
   cr += dr, cc += dc;
  if (!b[cr][cc])
  {
   res.clear ();
   return res;
  }
  res.insert (make_pair (cr, cc));
 }

 return res;
}

bool solve (int hr)
{
 queue <set <pair <int, int> > > q;
 set <set <pair <int, int> > > u;

 q.push (ls);
 u.insert (ls);
 while (!q.empty ())
 {
  set <pair <int, int> > s = q.front ();
  q.pop ();

  set <pair <int, int> >::iterator it;
  for (it = s.begin (); it != s.end (); it++)
   eprintf (" (%d, %d)", it -> first, it -> second);
  eprintf ("\n");
  for (it = s.begin (); it != s.end (); it++)
   if (it -> first < hr)
    break;
  if (it == s.end ())
   return true;

  set <pair <int, int> > t;
  t = move (s, +1,  0);
  if (t.size ())
   if (u.find (t) == u.end ())
   {
    u.insert (t);
    q.push (t);
   }
  t = move (s,  0, -1);
  if (t.size ())
   if (u.find (t) == u.end ())
   {
    u.insert (t);
    q.push (t);
   }
  t = move (s,  0, +1);
  if (t.size ())
   if (u.find (t) == u.end ())
   {
    u.insert (t);
    q.push (t);
   }
 }

 return false;
}

int main (void)
{
 int test, tests;
 int i, j, k, p;
 bool ok;

 scanf (" %d ", &tests);
 for (test = 1; test <= tests; test++)
 {
  memset (a, '#', sizeof (a));
  scanf (" %d %d", &r, &c);
  n = 0;
  for (i = 1; i <= r; i++)
   for (j = 1; j <= c; j++)
   {
    scanf (" %c", &a[i][j]);
    if ('0' <= a[i][j] && a[i][j] <= '9')
    {
     k = a[i][j] - '0';
     n = max (n, k + 1);
     vr[k] = i;
     vc[k] = j;
     a[i][j] = '.';
    }
   }


  printf ("Case #%d:\n", test);
  for (k = 0; k < n; k++)
  {
   ls.clear ();
   rs.clear ();

   memset (b, 0, sizeof (b));
   lr = vr[k];
   p = recur (vr[k], vc[k]);
   eprintf ("%d %d\n", lr, vr[k]);

   if (lr == vr[k])
    ok = true;
   else
    ok = solve (vr[k]);
   printf ("%d: %d %s\n", k, p, ok ? "Lucky" : "Unlucky");
  }
 }

 return 0;
}
