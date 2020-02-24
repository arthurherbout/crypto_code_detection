#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <set>
#include <map>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

#define forn(i, n) for (int i = 0; i < (int)(n); i++)

#define m 55

char a[m][m];
int w, h;

int di[4] = {1, -1, 0, 0};
int dj[4] = {0, 0, 1, -1};

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

    scanf("%d%d", &h, &w);
    forn (i, h)
      scanf("%s", a[i]);
    do
    {
      int be = -1, bi, bj;
      forn (i, h)
        forn (j, w)
          if (a[i][j] == '?')
          {
            int add = 4;
            forn (k, 4)
            {
              int ni = i + di[k], nj = j + dj[k];
              if (0 <= ni && ni < h && 0 <= nj && nj < w)
                if (a[ni][nj] == '#')
                  add -= 2;
            }
            if (add > be)
              be = add, bi = i, bj = j;
          }
      if (be <= 0)
        break;
      a[bi][bj] = '#';
    } while (true);

    int ans = 0;
    forn (i, h)
      forn (j, w)
        if (a[i][j] == '#')
          forn (k, 4)
          {
            int ni = i + di[k], nj = j + dj[k];
            if (0 <= ni && ni < h && 0 <= nj && nj < w)
            {
              if (a[ni][nj] != '#')
                ans++;
            }
            else
              ans++;
          }
    printf("%d", ans);
    fprintf(stderr, "%d\n", ans);

    fprintf(stderr, "Case #%d done\n", tt + 1);
    puts("");
  }
  return 0;
}
