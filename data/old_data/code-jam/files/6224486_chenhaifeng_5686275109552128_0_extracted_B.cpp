#include <set>
#include <map>
#include <queue>
#include <vector>
#include <string>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>
#include <algorithm>

using namespace std;

bool check(const vector<int>& cake, const int& mid) {
  // cout << "mid = " << mid << endl;
  std::priority_queue<int, std::vector<int>, std::less<int> > q;
  for (size_t i = 0; i < cake.size(); ++i) {
    q.push(cake[i]);
  }
  // cout << "top = " << q.top() << endl;
  int cut = 0;
  while (!q.empty()) {
    // cout << "now = " << q.top() +cut << " " << mid << endl;
    if (q.top() + cut <= mid)
      return true;
    else if (q.top() + cut > mid && q.top() > 1) {
      int tmp = q.top();
      q.pop();
      q.push((tmp + 1) / 2);
      q.push(tmp - (tmp + 1) / 2);
      cut++;
    } else
      return false;
  }
}

int main() {
  freopen("B-small-attempt5.in", "r", stdin);
  //freopen("B.in", "r", stdin);
  freopen("B-small.out", "w", stdout);

  int T;
  scanf("%d", &T);
  for (int t = 0; t < T; ++t) {
    //  cout << "test ..... " << t + 1 << endl;
    int n;
    cin >> n;
    vector<int> cake(n, 0);
    int maxCake = 0;
    for (int i = 0; i < n; ++i) {
      cin >> cake[i];
      if (cake[i] > maxCake)
        maxCake = cake[i];
    }

    int best = maxCake;
    int l = 1, h = maxCake;
    while (l < h) {
      int mid = (l + h) / 2;
      bool f = check(cake, mid);
      if (f == true) {
        h = mid;
      } else {
        l = mid + 1;
      }
    }
    printf("Case #%d: %d\n", t + 1, h);
  }

  return 0;
}
