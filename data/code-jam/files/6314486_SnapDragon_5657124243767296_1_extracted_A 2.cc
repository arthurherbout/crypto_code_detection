#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

map<int, vector<int>> vd;
vector<int> dv;
vector<bool> seen;

bool doit(int d, int v) {
  if (dv[d] == -1) {
    dv[d] = v;
    return true;
  }
  if (seen[d]) return false;
  seen[d] = true;
  const vector<int>& ds = vd[dv[d]];
  for (int i = 0; i < ds.size(); i++) if (ds[i] != d && doit(ds[i], dv[d])) {
    dv[d] = v;
    return true;
  }
  return false;
}


int main() {
  int T, prob=1;
  for (cin >> T; T--;) {
    int N;
    cin >> N;
    vd.clear();
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < 6; j++) {
        int x;
        cin >> x;
        vd[x].push_back(i);
      }
    }

    dv = vector<int>(N, -1);
    int ret = 0;
    int curs = -2, cure = -3;
    for (;;) {
      cure++;
      ret = max(ret, cure-curs);
      if (!vd.count(cure)) {
        auto it = vd.upper_bound(cure);
        if (it == vd.end()) break;
        dv = vector<int>(N, -1);
        curs = cure = it->first;
      }

      for (;;) {
        seen = vector<bool>(N);
        const vector<int>& ds = vd[cure];
        int i;
        for (i = 0; i < ds.size(); i++) if (doit(ds[i], cure)) break;
        if (i < ds.size()) break;
        const vector<int>& ds2 = vd[curs];
        for (i = 0; i < ds2.size(); i++) if (dv[ds2[i]] == curs) break;
        dv[ds2[i]] = -1;
        curs++;
      }
    }

    cout << "Case #" << prob++ << ": " << ret << endl;
  }
}
