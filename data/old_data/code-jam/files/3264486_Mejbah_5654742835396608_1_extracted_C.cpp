#include <bits/stdc++.h>

using namespace std;

int main() {
  int T;
  scanf("%d", &T);
  for (int cn = 1; cn <= T; cn++) {
    int n, k;
    scanf("%d%d", &n, &k);
    priority_queue<int> q;
    q.push(n);
    int l, r;
    for (int i = 0; i < k; i++) {
      int m = q.top() - 1;
      q.pop();
      l = m / 2;
      r = m - l;
      if (l > 0) {
        q.push(l);
      }
      if (r > 0) {
        q.push(r);
      }
    }
    if (l < r) {
      swap(l, r);
    }
    printf("Case #%d: %d %d\n", cn, l, r);
  }
}
