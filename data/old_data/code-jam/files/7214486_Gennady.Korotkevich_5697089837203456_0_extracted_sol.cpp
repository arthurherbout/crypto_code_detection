#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

long long f[1234567];

int main() {
  freopen("in", "r", stdin);
  freopen("out", "w", stdout);
  int tt;
  scanf("%d", &tt);
  for (int qq = 1; qq <= tt; qq++) {
    printf("Case #%d: ", qq);
    fflush(stdout);
    long long n, a, b;
    cin >> n >> a >> b;
    for (int i = 0; ; i++) {
      if (i < a) {
        f[i] = 1;
      } else {
        f[i] = f[i - a];
        if (i >= b) {
          f[i] += f[i - b];
        } else {
          f[i] += 1;
        }
      }
      if (f[i] >= n) {
        cout << i << endl;
        break;
      }
    }
    fflush(stdout);
  }
  return 0;
}
