#include <algorithm>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string> G;
vector<int> tx, ty, sx, sy;
int tv[101][101];

int main() {
  int T, X, Y, M, prob=1;
  for (cin >> T; T--;) {
    cin >> X >> Y >> M;
    G.resize(Y);
    for (int y = 0; y < Y; y++) cin >> G[y];

    memset(tv, 0, sizeof(tv));
    tx.clear(); ty.clear();
    for (int y = 0; y < Y; y++)
    for (int x = 0; x < X; x++) if (G[y][x] == 'T') {
      int n = tx.size();
      tx.push_back(x);
      ty.push_back(y);
      for (int x2 = x; x2 >= 0 && G[y][x2] != '#'; x2--) {
        tv[y][x2] |= (1<<n);
      }
      for (int x2 = x; x2 < X  && G[y][x2] != '#'; x2++) {
        tv[y][x2] |= (1<<n);
      }
      for (int y2 = y; y2 >= 0 && G[y2][x] != '#'; y2--) {
        tv[y2][x] |= (1<<n);
      }
      for (int y2 = y; y2 < Y  && G[y2][x] != '#'; y2++) {
        tv[y2][x] |= (1<<n);
      }
    }

    sx.clear(); sy.clear();
    for (int y = 0; y < Y; y++)
    for (int x = 0; x < X; x++) if (G[y][x] == 'S') {
      sx.push_back(x);
      sy.push_back(y);
    }

    int nt = tx.size(), ns = sx.size();

    vector<vector<int>> skill(ns, vector<int>(1<<nt));
    for (int b = 0; b < (1<<nt); b++)
    for (int s = 0; s < ns; s++) {
      vector<vector<bool>> u(Y, vector<bool>(X));
      vector<pair<int, int>> q, q2;
      q.emplace_back(sx[s], sy[s]);
      u[sy[s]][sx[s]] = true;
      for (int steps = 0; steps <= M; steps++) {
        q2.clear();
        for (int i = 0; i < q.size(); i++) {
          int x = q[i].first, y = q[i].second;
//cout << s << " at " << x << ',' << y << ' ' << steps << " " << b << endl;
          if (tv[y][x] & ~b) {
            skill[s][b] |= (tv[y][x] & ~b);
          } else {
            if (x-1 >= 0 && !u[y][x-1] && G[y][x-1] != '#') {
              q2.emplace_back(x-1, y);
              u[y][x-1] = true;
            }
            if (x+1 < X  && !u[y][x+1] && G[y][x+1] != '#') {
              q2.emplace_back(x+1, y);
              u[y][x+1] = true;
            }
            if (y-1 >= 0 && !u[y-1][x] && G[y-1][x] != '#') {
              q2.emplace_back(x, y-1);
              u[y-1][x] = true;
            }
            if (y+1 < Y  && !u[y+1][x] && G[y+1][x] != '#') {
              q2.emplace_back(x, y+1);
              u[y+1][x] = true;
            }
          }
        }
        q = q2;
      }
    }

    vector<pair<int, int>> ret;
    vector<int> perm(max(ns, nt));
    for (int i = 0; i < nt; i++) perm[i] = i;
    for (int i = nt; i < ns; i++) perm[i] = nt;
    do {
      vector<int> u(ns);
      vector<pair<int, int>> cur;
      bool change = true;
      int b = 0;
      while (change) {
        change = false;
        for (int s = 0; s < ns; s++) if (!u[s]) {
          if (skill[s][b] & (1<<perm[s])) {
            change = true;
            u[s] = true;
            b |= (1<<perm[s]);
            cur.push_back(make_pair(s+1, perm[s]+1));
          }
        }
      }
      if (cur.size() > ret.size()) ret = cur;
    } while (next_permutation(perm.begin(), perm.end()));

    cout << "Case #" << prob++ << ": " << ret.size() << endl;
    for (int i = 0; i < ret.size(); i++) {
      cout << ret[i].first << ' ' << ret[i].second << endl;
    }
  }
}
