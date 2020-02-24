#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

typedef vector<int> vi;

bool check(int V, const vi& d) {
  vi ok(V+1, 0);
  ok[0] = 1;
  for (int x : d) {
    for (int v = V-x; v >= 0; --v) {
      ok[v+x] |= ok[v];
    }
  }
  for (int i = 0; i <= V; ++i) if (!ok[i]) return false;
//  int D = d.size();
//  for (int v = 1; v < V; ++v) {
//    int x = v;
//    for (int j = D-1; j >= 0; --j)
//      if (x >= d[j]) x -= d[j];
//    if (x) return false;
//  }
  return true;
}

int solve() {
  int C, D, V; cin >> C >> D >> V;
  vi d(D);
  for (int& x : d) cin >> x;
  if (check(V, d)) return 0;
  for (int a = 1; a < V; ++a) {
    vi t = d;
    t.push_back(a);
    inplace_merge(t.begin(), t.end()-1, t.end());
    if (check(V, t)) return 1;
  }
  for (int a = 1; a < V; ++a) {
    for (int b = a+1; b < V; ++b) {
      vi t = d; 
      t.push_back(a);
      t.push_back(b);
      inplace_merge(t.begin(), t.end()-2, t.end());
      if (check(V, t)) return 2;
    }
  }
  for (int a = 1; a < V; ++a) {
    for (int b = a+1; b < V; ++b) {
      for (int c = b+1; c < V; ++c) {
        vi t = d; 
        t.push_back(a);
        t.push_back(b);
        t.push_back(c);
        inplace_merge(t.begin(), t.end()-3, t.end());
        if (check(V, t)) return 3;
      }
    }
  }
  for (int a = 1; a < V; ++a) {
    for (int b = a+1; b < V; ++b) {
      for (int c = b+1; c < V; ++c) {
        for (int e = c+1; e < V; ++e) {
          vi t = d; 
          t.push_back(a);
          t.push_back(b);
          t.push_back(c);
          t.push_back(e);
          inplace_merge(t.begin(), t.end()-4, t.end());
          if (check(V, t)) return 4;
        }
      }
    }
  }
  return 5;
}

int main() {
  int T; cin >> T;
  for (int test = 1; test <= T; ++test) {
    cout << "Case #" << test << ": " << solve() << endl;
  }
}
