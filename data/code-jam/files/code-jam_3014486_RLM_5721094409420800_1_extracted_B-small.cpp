#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define f(x, y) for(int x = 0; x < (int)y; ++x)

typedef vector<int> vi;

int solve() {
  int N; cin >> N;
  vi v(N);
  f(i, N) cin >> v[i];
  int x1 = 0, x2 = N;
  int r = 0;
  while (x1 < x2) {
    int j = min_element(v.begin()+x1, v.begin()+x2)-v.begin();
    if (j-x1 > x2-1-j) {
      while (++j < x2) {
        swap(v[j-1], v[j]);
        ++r;
      }
      --x2;
    }
    else {
      while (j-- > x1) {
        swap(v[j], v[j+1]);
        ++r;
      }
      ++x1;
    }
  }
  return r;
}

int main() {
  int T; cin >> T;
  for (int C = 1; C <= T; ++C) {
    cout << "Case #" << C << ": " << solve() << endl;
  }
}
