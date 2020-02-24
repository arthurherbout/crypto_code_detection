#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <map>
using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;
typedef vector<pii> vii;
typedef vector<string> vs;

int main() {
  int T;
  cin >> T;
  for (int test = 1; test <= T; ++test) {
    printf("Case #%d: ", test);
    int n,l;
    cin >> n >> l;
    vs g(n);
    string b;
    for (int i = 0; i < n; ++i) cin >> g[i];
    cin >> b;
    sort(g.begin(), g.end());
    if (binary_search(g.begin(), g.end(), b)) {
      cout << "IMPOSSIBLE\n";
      continue;
    }
//    cerr << l << ' ' << b << endl;
    if (b == string(l, '1')) {
      string s(2*l, '0');
      for (int i = 1; i < (int)s.size(); i += 2) s[i] = '?';
      string t(l-1, '1');
      if (l == 1) t = string(1, '0');
      cout << s << ' ' << t << endl;
      assert(s.size() + t.size() <= 200);
      continue;
    }
  }
  return 0;
}
