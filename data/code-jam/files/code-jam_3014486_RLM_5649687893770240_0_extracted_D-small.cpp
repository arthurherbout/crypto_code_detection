#include <iostream>
#include <vector>
#include <set>
using namespace std;

#define f(x, y) for(int x = 0; x < (int)y; ++x)

typedef vector<int> vi;
typedef vector<string> vs;
typedef set<string> ss;
typedef vector<ss> vss;

int M, N, X, Y;
vs v;
vi p;

void update() {
  vss pref(N);
  f(i, M) f(j, v[i].size()+1) pref[p[i]].insert(v[i].substr(0, j));
  int x = 0;
  f(i, N) {
    if (pref[i].empty()) return;
    x += pref[i].size();
  }
  if (x > X) {
    X = x;
    Y = 1;
  }
  else if (x == X) ++Y;
}

void bt(int k) {
  if (k == M) return update();
  f(i, N) {
    p[k] = i;
    bt(k+1);
  }
}

void solve() {
  cin >> M >> N;
  v = vs(M); f(i, M) cin >> v[i];
  p = vi(M);
  X = Y = 0;
  bt(0);
}

int main() {
  int T; cin >> T;
  for (int C = 1; C <= T; ++C) {
    solve();
    cout << "Case #" << C << ": " << X << ' ' << Y << endl;
  }
}
