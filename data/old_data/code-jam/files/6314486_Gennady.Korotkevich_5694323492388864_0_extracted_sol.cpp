#include <bits/stdc++.h>

using namespace std;

const int MAX = 10000;
const int N = 105;

int x[N], y[N], z[N];
int qx[N * MAX], qy[N * MAX], qz[N * MAX];
int dist[N][N];
set <int> notwas[N];

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    int n;
    scanf("%d", &n);
    for (int i = 0; i < n + 2; i++) {
      scanf("%d %d %d", x + i, y + i, z + i);
    }
    for (int i = 0; i < n + 2; i++) {
      for (int j = 0; j < n + 2; j++) {
        dist[i][j] = abs(x[i] - x[j]) + abs(y[i] - y[j]) + abs(z[i] - z[j]);
      }
    }
    if (n == 1) {
      if (dist[0][2] == dist[1][2]) {
        printf("%d\n", 1);
      } else {
        puts("IMPOSSIBLE");
      }
      continue;
    }
    for (int i = 2; i < n + 2; i++) {
      notwas[i].clear();
      for (int j = 0; j < MAX; j++) {
//        was[i][j] = false;
        notwas[i].insert(j);
      }
    }
    int e = 0;
    for (int i = 2; i < n + 2; i++) {
      qx[e] = i;
      qy[e] = dist[0][i];
      qz[e] = 1;
      e++;
      notwas[i].erase(dist[0][i]);
    }
    int ans = -1;
    int b = 0;
    while (b < e) {
      if (qy[b] == dist[qx[b]][1]) {
        ans = qz[b];
        break;
      }
      for (int j = 2; j < n + 2; j++) {
        int d = dist[qx[b]][j];
        int from = abs(qy[b] - d);
        int to = qy[b] + d;
        while (true) {
          auto it = notwas[j].lower_bound(from);
          if (it == notwas[j].end()) {
            break;
          }
          if ((*it) > to) {
            break;
          }
          int yy = *it;
          qx[e] = j;
          qy[e] = yy;
          qz[e] = qz[b] + 1;
          e++;
          notwas[j].erase(it);
        }
      }
      b++;
    }
    assert(ans != -1);
    printf("%d\n", ans);
    cerr << "test " << qq << "/" << tt << " solved, time = " << clock() << " ms" << endl;
  }
  return 0;
}
