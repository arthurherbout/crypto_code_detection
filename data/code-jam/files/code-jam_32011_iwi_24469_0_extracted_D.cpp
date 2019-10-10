#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <cmath>
using namespace std;

#define all(c) ((c).begin()), ((c).end()) 
#define iter(c) __typeof((c).begin())
#define present(c, e) ((c).find((e)) != (c).end()) 
#define cpresent(c, e) (find(all(c), (e)) != (c).end())
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define tr(c, i) for (iter(c) i = (c).begin(); i != (c).end(); ++i)
#define pb(e) push_back(e)
#define mp(a, b) make_pair(a, b)

typedef long long ll;

const int dx[] = {1, -1, 0, 0};
const int dy[] = {0, 0, -1, 1};

int W, H;
char fld[50][50];

bool vis[50][50];
int prvx[50][50];
int prvy[50][50];

bool usd[50][50];

int num[50][50], num2[50][50];

void bfs(int sx, int sy) {
  queue<pair<int, int> > que;
  memset(vis, 0, sizeof(vis));
  que.push(mp(sx, sy));
  vis[sx][sy] = 1;
  num[sx][sy] = 0;

  while (!que.empty()) {
    int x = que.front().first;
    int y = que.front().second;
    que.pop();

    rep (i, 4) {
      int tx = x + dx[i];
      int ty = y + dy[i];
      if (tx < 0 || W <= tx || ty < 0 || H <= ty) continue;

      if (!vis[tx][ty] && fld[tx][ty] != '.') {
        vis[tx][ty] = true;
        num[tx][ty] = num[x][y] + 1;
        que.push(mp(tx, ty));
        prvx[tx][ty] = x;
        prvy[tx][ty] = y;
        //printf("%d  %d %d -- %d %d\n", num[tx][ty], tx, ty, x, y);
      }
    }
  }
}

void solve() {
  scanf("%d%d", &H, &W);
  rep (y, H) rep (x, W) scanf(" %c", &fld[x][y]);

  bfs(0, 0);
  int sx = -1, sy = -1;
  rep (y, H) rep (x, W) {
    if (mp(y, x) != mp(0, 0) && fld[x][y] == 'T') {
      sx = x;
      sy = y;
    }
  }

  int tx = sx, ty = sy;

  memset(usd, 0, sizeof(usd));
  int ans = 0;
  while (mp(tx, ty) != mp(0, 0)) {
    //printf("awef %d %d\n", tx, ty);
    ans += num[tx][ty];
    usd[tx][ty] = true;

    int xx = prvx[tx][ty];
    int yy = prvy[tx][ty];
    tx = xx;
    ty = yy;
  }
  

  if (mp(sx, sy) != mp(-1, -1)) {
    memcpy(num2, num, sizeof(num2));
    bfs(sx, sy);
    rep (y, H) rep (x, W) {
      if (usd[x][y] || fld[x][y] == '.') continue;
      ans += min(num[x][y], num2[x][y]);
      //printf("adding %d %d: %d %d\n", x, y, num[x][y], num2[x][y]);
    }
  }
  else {
    rep (y, H) rep (x, W) {
      if (usd[x][y] || fld[x][y] == '.') continue;
      ans += num[x][y];
    }
  }
  
  printf("%d\n", ans);
}


int main() {
  int cases;
  scanf("%d", &cases);
  rep (ca, cases) {
    printf("Case #%d: ", ca + 1);
    solve();
  }
  return 0;
}

