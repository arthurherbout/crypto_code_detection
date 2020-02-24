#ifdef DEBUG
//#define _GLIBCXX_DEBUG
#endif
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <deque>
#include <string>
#include <cstring>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <ctime>
#include <cassert>
#include <functional>
#include <complex>

using namespace std;
typedef long double LD;
typedef long long LL;

int p[15];

char f[35][35];
int ff[35][35];
int can[1 << 10][10];
bool was[35][35];

vector<pair<int, int>> t;
vector<pair<int, int>> s;

int dx[] = {0, 1, 0, -1};
int dy[] = {1, 0, -1, 0};
int c, r, m;

int DFS(int x, int y, int m) {
  if (ff[x][y]) {
    return ff[x][y];
  }
  if (!m) {
    return 0;
  }
  was[x][y] = true;
  int ret = 0;
  for (int i = 0; i < 4; ++i) {
    int nx = x + dx[i], ny = y + dy[i];
    if (nx < 0 || nx >= r || ny < 0 || ny >= c || f[nx][ny] == '#' || was[nx][ny]) {
      continue;
    }
    ret |= DFS(nx, ny, m - 1);
  }
  return ret;
}

void Solve() {
  cin >> c >> r >> m;
  for (int i = 0; i < r; ++i) {
    cin >> f[i];
  }

  t.clear();
  s.clear();

  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      if (f[i][j] == 'T') {
        t.emplace_back(i, j);
      } else if (f[i][j] == 'S') {
        s.emplace_back(i, j);
      }
    }
  }

  for (int mask = 0; mask < (1 << t.size()); ++mask) {
    memset(ff, 0, sizeof ff);
    for (int tur = 0; tur < t.size(); ++tur) {
      if (mask & (1 << tur)) {
        for (int d = 0; d < 4; ++d) {
          int x = t[tur].first, y = t[tur].second;
          while (x >= 0 && x < r && y >= 0 && y < c) {
            if (f[x][y] == '#') {
              break;
            }
            ff[x][y] |= 1 << tur;
            x += dx[d];
            y += dy[d];
          }
        }
      }
    }
    for (int sol = 0; sol < s.size(); ++sol) {
      memset(was, 0, sizeof was);
      can[mask][sol] = DFS(s[sol].first, s[sol].second, m);
      //cerr << "mask: " << mask << " sol: " << sol << " can: " << can[mask][sol] << endl;
    }
  }

  int ba = 0;
  vector<int> p, bp, bs;
  for (int i = 0; i < t.size(); ++i) {
    p.push_back(i);
  }

  do {
    vector<int> ss;
    int q = 0;
    int mask = (1 << t.size()) - 1;
    for (int i = 0; i < s.size() && q < t.size(); ++i) {
      if (can[mask][i] & (1 << p[q])) {
        //cerr << mask << " " << i << " " << p[q] << endl;
        ss.push_back(i);
        mask ^= (1 << p[q]);
        ++q;
      }
    }
    if (q > ba) {
      ba = q;
      bp = p;
      bs = ss;
    }
  } while (next_permutation(p.begin(), p.end()));
  cout << ba << endl;
  for (int i = 0; i < ba; ++i) {
    cout << bs[i] + 1 << " " << bp[i] + 1 << endl;
  }
}

int main() {
    freopen("d.in", "r", stdin);
    freopen("d.out", "w", stdout);
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i) {
        printf("Case #%d: ", i + 1);
        Solve();
    }
    return 0;
}
