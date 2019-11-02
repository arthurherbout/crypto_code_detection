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

#define first x
#define second y

const int MaxN = 20, MaxC = 0x3F3F3F3F, NA = -1, MOD = 1000003;
const int MaxD = 8, MedD = MaxD >> 1, AndD = MedD - 1;
const int dx [MaxD] = {-1, -1, -1,  0,  1,  1,  1,  0},
          dy [MaxD] = {-1,  0,  1,  1,  1,  0, -1, -1};

int a [MaxN] [MaxN];
int b [MaxN] [MaxN];
int m, n, res;

void norm (int & x, int & y)
{
 if (x < 0) x += m;
 if (x >= m) x -= m;
 if (y < 0) y += n;
 if (y >= n) y -= n;
}

bool check (void)
{
 bool c [MaxN] [MaxN];
 int x, y, cx, cy;
 memset (c, 0, sizeof (c));
 for (x = 0; x < m; x++)
  for (y = 0; y < n; y++)
  {
   cx = x + dx[a[x][y] | b[x][y]];
   cy = y + dy[a[x][y] | b[x][y]];
   norm (cx, cy);
   if (c[cx][cy])
    return false;
   c[cx][cy] = true;
  }
 return true;
}

void recur (int x, int y)
{
 int nx, ny;
 if (x >= m)
 {
  res += check ();
  return;
 }
 nx = x;
 ny = y + 1;
 if (ny >= n)
 {
  ny = 0;
  nx++;
 }
 b[x][y] = 0;
 recur (nx, ny);
 b[x][y] = MedD;
 recur (nx, ny);
}

int main (void)
{
 int test, tests;
 int i, j;
 char ch;
 scanf (" %d", &tests);
 for (test = 1; test <= tests; test++)
 {
  scanf (" %d %d", &m, &n);
  for (i = 0; i < m; i++)
   for (j = 0; j < n; j++)
   {
    scanf (" %c", &ch);
    switch (ch)
    {
     case '\\': a[i][j] = 0; break;
     case  '|': a[i][j] = 1; break;
     case  '/': a[i][j] = 2; break;
     case  '-': a[i][j] = 3; break;
     default: assert (false); break;
    }
   }
  res = 0;
  recur (0, 0);
  printf ("Case #%d: %d\n", test, res);
 }
 return 0;
}
