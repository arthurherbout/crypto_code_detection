#include <cstdio>
#include <map>
#include <tuple>
#include <vector>
using namespace std;
int main() {
  int Ti, Tn;
  scanf("%d", &Tn);
  for (Ti = 1; Ti <= Tn; ++Ti) {
    int J, P, S, K;
    scanf("%d %d %d %d", &J, &P, &S, &K);
    vector<tuple<int, int, int>> v;
    v.reserve(J * P * S);
    for (int i = 1; i <= J; ++i)
      for (int j = 1; j <= P; ++j)
        for (int k = 1; k <= S; ++k)
          v.emplace_back(i, j, k);
    int mc = 1, mi = 1;
    for (int i = 2; i < (1 << v.size()); ++i) {
      if (__builtin_popcount(i) <= mc)
        continue;
      map<pair<int, int>, int> JP, JS, SP;
      int j = i;
      bool valid = true;
      int cnt = 0;
      for (auto &it : v) {
        if (j & 1) {
          tie(J, P, S) = it;
          if (++JP[{J, P}] > K) {
            valid = false;
            break;
          }
          if (++JS[{J, S}] > K) {
            valid = false;
            break;
          }
          if (++SP[{S, P}] > K) {
            valid = false;
            break;
          }
          ++cnt;
        }
        j >>= 1;
      }
      if (!valid)
        continue;
      if (mc < cnt)
        mc = cnt, mi = i;
    }
    printf("Case #%d: %d\n", Ti, mc);
    for (auto &it : v) {
      if (mi & 1) {
        tie(J, P, S) = it;
        printf("%d %d %d\n", J, P, S);
      }
      mi >>= 1;
    }
  }
}