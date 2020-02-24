#include <algorithm>
#include <bitset>
#include <cassert>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <deque>
#include <functional>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

void Solve() {
  int N, L;
  cin >> N >> L;

  vector<string> A(N);
  for (auto& a : A) {
    cin >> a;
  }

  string B;
  cin >> B;

  if (find(A.begin(), A.end(), B) != A.end()) {
    cout << "IMPOSSIBLE" << endl;
    return;
  }

  int max_one = 0;
  for (const auto& a : A) {
    int one = count(a.begin(), a.end(), '1');
    max_one = max(max_one, one);
  }

  if (max_one == 0) {
    cout << "0" << ' ' << "?" << endl;
    return;
  }

  string left(max_one, '1');
  string right;
  for (int i = 0; i < L; ++i) {
    right += "?0";
  }

  assert(!left.empty());
  cout << left << ' ' << right << endl;
}

int main() {
//  freopen("../Console/1.txt", "rb", stdin);
  freopen("../Console/D-small-attempt4.in", "rb", stdin);
  freopen("../Console/D-small-attempt4.out", "wb", stdout);
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 0; tc < T; ++tc) {
    cout << "Case #" << tc + 1 << ": ";
    Solve();
  }

  return 0;
}
