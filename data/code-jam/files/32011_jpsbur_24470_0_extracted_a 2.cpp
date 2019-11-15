#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)
#define mp make_pair

int w, h;
int x0, y0, x1, y1, x2, y2;

bool ok( int x, int y )
{
  return 0 <= x && x < w && 0 <= y && y < h;
}

#define m 110

int u[m][m];

void go( int x, int y )
{
  if (!ok(x, y) || u[x][y])
    return;
  u[x][y] = 1;
  go(x + x1, y + y1);
  go(x + x2, y + y2);
}

int main()
{
  freopen("a.in", "r", stdin);
  freopen("a.out", "w", stdout);
  int nt;
  scanf("%d", &nt);
  forn (tt, nt)
  {
    fprintf(stderr, "Case #%d:\n", tt + 1);
    printf("Case #%d: ", tt + 1);

    scanf("%d%d", &w, &h);
    scanf("%d%d%d%d", &x1, &y1, &x2, &y2);
    scanf("%d%d", &x0, &y0);
    memset(u, 0, sizeof(u));
    go(x0, y0);
    int tans = 0;
    forn (i, w)
      forn (j, h)
        tans += u[i][j];

    int ans = 0;
    if (x1 * y2 == x2 * y1)
    {
      set <pair <int, int> > a, b;
      a.insert(mp(x0, y0));
      b.insert(mp(x0, y0));
      while (a.size() > 0)
      {
        int xx = a.begin()->first;
        int yy = a.begin()->second;
        a.erase(a.begin());
        if (ok(xx + x1, yy + y1) && !b.count(mp(xx + x1, yy + y1)))
        {
          a.insert(mp(xx + x1, yy + y1));
          b.insert(mp(xx + x1, yy + y1));
        }
        if (ok(xx + x2, yy + y2) && !b.count(mp(xx + x2, yy + y2)))
        {
          a.insert(mp(xx + x2, yy + y2));
          b.insert(mp(xx + x2, yy + y2));
        }
      }
      ans = b.size();
    }
    else
    {
      for (int t = 0; t < 2 * max(w, h) + 2; t++)
      {
        int xx = x0 + t * x1, yy = y0 + t * y1;
        int mi = 0, ma, av;
        if (xx < 0)
        {
          if (x2 <= 0)
            continue;
          mi = max(mi, (-xx + x2 - 1) / x2);
        }
        if (xx >= w)
        {
          if (x2 >= 0)
            continue;
          mi = max(mi, (xx - w + 1 - x2 - 1) / (-x2));
        }
        if (yy < 0)
        {
          if (y2 <= 0)
            continue;
          mi = max(mi, (-yy + y2 - 1) / y2);
        }
        if (yy >= h)
        {
          if (y2 >= 0)
            continue;
          mi = max(mi, (yy - h + 1 - y2 - 1) / (-y2));
        }
        xx += x2 * mi, yy += y2 * mi;
        if (!ok(xx, yy))
          continue;
        if (mi != 0)
          fprintf(stderr, "!");
        mi = 0, ma = 2 * (w + h) + 2;
        while (mi < ma)
        {
          av = (mi + ma + 1) / 2;
          if (ok(xx + av * x2, yy + av * y2))
            mi = av;
          else
            ma = av - 1;
        }
        ans += mi + 1;
      }
    }
    printf("%d", tans);
    fprintf(stderr, "%d %d\n", ans, tans);
//    if (ans != tans)
//      return 1;

    fprintf(stderr, "Case #%d done\n", tt + 1);
    puts("");
  }
  return 0;
}
