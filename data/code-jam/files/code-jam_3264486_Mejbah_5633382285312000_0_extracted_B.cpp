#include <bits/stdc++.h>

using namespace std;

bool isTidy(int n) {
  vector<int> v;
  while (n) {
    v.push_back(n%10);
    n /= 10;
  }
  for (int i = (int)v.size() - 1; i > 0; i--) {
    if (v[i] > v[i - 1]) {
      return 0;
    }
  }
  return 1;
}

int main() {
  int T;
  scanf("%d", &T);
  for (int cn = 1; cn <= T; cn++) {
    int N;
    scanf("%d", &N);
    while (N) {
      if (isTidy(N)) {
        break;
      }
      N--;
    }
    printf("Case #%d: %d\n", cn, N);
  }
}
