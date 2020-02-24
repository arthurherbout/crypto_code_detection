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

int W, H;

inline bool valid(int x, int y) {
  return 0 <= x && x < W && 0 <= y && y < H;
}

void solve() {
  int x1, y1, x2, y2, sx, sy;
  cin >> W >> H;
  cin >> x1 >> y1;
  cin >> x2 >> y2;
  cin >> sx >> sy;
  
  vector<vector<int> > vis(W, H);

  queue<pair<int, int> > que;
  que.push(mp(sx, sy));
  vis[sx][sy] = 1;

  while (!que.empty()) {
    int x = que.front().first;
    int y = que.front().second;
    que.pop();

    if (valid(x + x1, y + y1) && !vis[x + x1][y + y1]) {
      vis[x + x1][y + y1] = 1;
      que.push(mp(x + x1, y + y1));
    }
    if (valid(x + x2, y + y2) && !vis[x + x2][y + y2]) {
      vis[x + x2][y + y2] = 1;
      que.push(mp(x + x2, y + y2));
    }
  }

  int ans = 0;
  rep (x, W) rep (y, H) ans += vis[x][y];
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

