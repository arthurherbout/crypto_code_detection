#include <bits/stdc++.h>

using namespace std;

char S[1003];
int L, K, N;

int main() {
  int T;
  scanf("%d", &T);
  for (int cn = 1; cn <= T; cn++) {
    scanf("%s%d", S, &K);
    L = strlen(S);
    N = L - K;
    int a = 0;
    for (int i = 0; i <= N; i++) {
      if (S[i] == '-') {
        for (int j = i; j < i + K; j++) {
          if (S[j] == '-') {
            S[j] = '+';
          } else {
            S[j] = '-';
          }
        }
        a++;
      }
    }
    bool f = 1;
    for (int i = N + 1; i < L; i++) {
      if (S[i] == '-') {
        f = 0;
        break;
      }
    }
    if (f) {
      printf("Case #%d: %d\n", cn, a);
    } else {
      printf("Case #%d: IMPOSSIBLE\n", cn);
    }
  }
}
