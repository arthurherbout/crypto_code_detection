#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

bool doit(vector<vector<pair<int, int>>> v) {
  bool change = true;
  while (change) {
    change = false;
    for (int i = 0; i < v.size(); i++) if (v[i].size())
    for (int j = i+1; j < v.size(); j++) if (v[j].size())
    if (v[j].back().second == v[i].back().second) {
      if (v[i].back().first < v[j].back().first) {
        v[i].pop_back();
      } else {
        v[j].pop_back();
      }
      change = true;
    }
  }
/*for (int i = 0; i < v.size(); i++) {
cout << '[';
for (int j = 0; j < v[i].size(); j++) cout << ' ' << v[i][j].first << ',' << v[i][j].second;
cout << " ]" << endl;
}
cout << endl;*/
  bool done = true;
  for (int i = 0; i < v.size(); i++) if (v[i].size() > 1) done = false;
  if (done) return true;

  int empty;
  for (empty = 0; empty < v.size(); empty++) if (v[empty].size() == 0) break;
  if (empty == v.size()) return false;
  for (int i = 0; i < v.size(); i++) if (v[i].size() > 1) {
    v[empty].push_back(v[i].back());
    v[i].pop_back();
    if (doit(v)) return true;
    v[i].push_back(v[empty].back());
    v[empty].pop_back();
  }
  
  return false;
}

int main() {
  int P;
  vector<vector<pair<int, int>>> p;
  cin >> P;
  p.resize(P);
  for (int i = 0; i < P; i++) {
    int c;
    cin >> c;
    p[i].resize(c);
    for (int j = 0; j < c; j++) {
      cin >> p[i][j].first >> p[i][j].second;
    }
    reverse(p[i].begin(), p[i].end());
  }

  int T, prob=1;
  for (cin >> T; T--;) {
    int N, C;
    cin >> N >> C;
    vector<vector<pair<int, int>>> v(N);
    for (int i = 0; i < N; i++) {
      int j;
      cin >> j;
      v[i] = p[j];
    }
    cout << "Case #" << prob++ << ": " << (doit(v) ? "POSSIBLE" : "IMPOSSIBLE") << endl;
  }
}
