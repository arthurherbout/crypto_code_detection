#include <iostream>
#include <fstream>

#ifdef LOCAL
#define eprintf(...) fprintf(stderr,__VA_ARGS__)
#else
#define eprintf(...)
#endif

#define TIMESTAMP(x) eprintf("["#x"] Time : %.3lf s.\n", clock()*1.0/CLOCKS_PER_SEC)
#define TIMESTAMPf(x, ...) eprintf("[" x "] Time : %.3lf s.\n", __VA_ARGS__, clock()*1.0/CLOCKS_PER_SEC)

using namespace std;

typedef long long ll;
typedef long double ld;

void PreCalc() {
}

ll a[210][210];
bool fix[200][200];

const ll INF = 1LL << 60;

void solve() {
  int r, c, n, d;
  scanf("%d%d%d%d", &r, &c, &n, &d);
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      a[i][j] = INF;
      fix[i][j] = false;
    }
  }
  for (int i = 0; i < n; i++) {
    int x, y, v;
    scanf("%d%d%d", &x, &y, &v);
    a[x - 1][y - 1] = v;
    fix[x - 1][y - 1] = true;
  }

  const int dx[4] = {0, 1, 0, -1};
  const int dy[4] = {1, 0, -1, 0};

  while (true) {
    bool any = false;
    for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
        ll mv = INF;
        for (int k = 0; k < 4; k++) {
          int ni = i + dx[k];
          int nj = j + dy[k];
          if (ni < 0 || nj < 0 || ni >= r || nj >= c)
            continue;
          mv = min(mv, a[ni][nj]);
        }
        if (a[i][j] > mv + d) {
          if (!fix[i][j]) {
            a[i][j] = mv + d;
            any = true;
          } else {
            printf("IMPOSSIBLE\n");
            return;
          }
        }
      }
    }
    if (!any) break;
  }

  const int MOD = 1000000007;

  int ans = 0;
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < c; j++) {
      ans = (ans + a[i][j]) % MOD;
    }
  }

  printf("%d\n", ans);
}


int main() {
//  freopen("d.in", "r", stdin);
//  freopen("d.out", "w", stdout);

  PreCalc();
  TIMESTAMP(PreCalc);

  char buf[1000];
  int testNum;
  fgets(buf, sizeof buf, stdin);
  sscanf(buf, "%d", &testNum);

  for (int testId = 1; testId <= testNum; testId++) {
    if (testId <= 20 || testId >= testNum - 20 || testId % 10 == 0)
      TIMESTAMPf("Test %d", testId);
    printf("Case #%d: ", testId);
    solve();
  }

  TIMESTAMP(end);
  return 0;
}