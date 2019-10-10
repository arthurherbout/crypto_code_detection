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

const int INF = 999999999;

typedef long long ll;

const int dx[] = {0, -1, 1, 0};
const int dy[] = {-1, 0, 0, 1};

int W, H, X, Y;
int fld[10000][10][10];

int saiki(int r) {

  if (fld[r][X][Y] == 0) return 0;

  for (int d = 0; d < 4; d++)
    if (fld[r][X + dx[d]][Y + dy[d]] > 0) goto dmp;

  return INF; // done
 dmp:

  /*
  if ( r <= 10) {
    printf("r: %d\n", r);
  for (int y = 1; y <= H; y++) {
      for (int x = 1; x <= W; x++) {
        printf("%d ", fld[r][x][y]);
      }
      puts("");
    }puts("");
    }
  */

  int res = 0;
  for (int d = 0; d < 4; d++) {
    memset(fld[r + 1], 0, sizeof(fld[r + 1]));
    for (int y = 1; y <= H; y++) {
      for (int x = 1; x <= W; x++) {
        fld[r + 1][x][y] += fld[r][x][y];

        if (x == X && y == Y) {
          fld[r + 1][x + dx[d]][y + dy[d]] -= fld[r][x][y];
        }
        else {
          int mad = 0, mas = 0;
          rep (i, 4) {
            if (fld[r][x + dx[i]][y + dy[i]] > mas) {
              mas = fld[r][x + dx[i]][y + dy[i]];
              mad = i;
            }
          }
          fld[r + 1][x + dx[mad]][y + dy[mad]] -= fld[r][x][y];
        }
      }
    }


    for (int y = 1; y <= H; y++) {
      for (int x = 1; x <= W; x++) {
        fld[r + 1][x][y] = max(0, fld[r + 1][x][y]);
      }
    }
    res = max(res, 1 + saiki(r + 1));
  }
  return res;
}

int main() {
  int T;
  scanf("%d", &T);
  for (int t = 1; t <= T; t++) {
    scanf("%d%d%d%d", &W, &H, &X, &Y);

    for (int y = 1; y <= H; y++)
      for (int x = 1; x <= W; x++)
        scanf("%d", &fld[0][x][y]);

    int res = saiki(0);

    printf("Case #%d: ", t);
    if (res >= INF) puts("forever");
    else printf("%d day(s)\n", max(1, res - 1));
  }
}

