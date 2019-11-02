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

int W, H;
int num[10][10];
int bom[10][10];

int getbom(int x, int y) {
  if (x < 0 || W <= x || y < 0 || H <= y) return 0;
  else return bom[x][y];
}

const int dx[9] = {1, 1, 1, 0, 0, 0, -1, -1, -1};
const int dy[9] = {1, 0, -1, 1, 0, -1, 1, 0, -1};

void solve() {
  scanf("%d%d", &H, &W);
  rep (y, H) rep (x, W) scanf("%d", &num[x][y]);
  int ans = 0;

  rep (bw, 1 << W) {
    rep (x, W) bom[x][0] = ((bw >> x) & 1);
    rep (bh, 1 << H) {
      rep (y, H) bom[0][y] = ((bh >> y) & 1);

      for (int x = 1; x < W; x++) {
        for (int y = 1; y < H; y++) {
          bom[x][y] = 0;

          int cnt = 0;
          rep (i, 9) cnt += getbom(x - 1 + dx[i], y - 1 + dy[i]);

          if (num[x - 1][y - 1] == cnt + 1)
            bom[x][y] = 1;
          else if (num[x - 1][y - 1] == cnt)
            bom[x][y] = 0;
          else 
            goto dmp;
        }
      }

      rep (x, W) rep (y, H) {
        int cnt = 0;
        rep (i, 9) cnt += getbom(x + dx[i], y + dy[i]);
        if (num[x][y] != cnt) goto dmp;
      }

      {
        /*
        rep (y, H) {
          rep (x, W) printf("%d", bom[x][y]);
          puts("");
        }
        puts("");
        */
        int tmp = 0;
        rep (x, W) tmp += bom[x][H / 2];
        ans = max(tmp, ans);
      }
    dmp:
      ;
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

