#include <iostream>
#include <vector>
using namespace std;

typedef vector<double> vd;

vd p, r;
int N, K;
double res;

void bt(int n, int k) {
  if (n == N and k == K) {
    res = max(res, r[K/2]);
    return;
  }
  if (n == N) return;
  bt(n+1, k);
  vd tmp = r;
  for (int i = 0; i <= K/2; ++i) {
    r[i] = tmp[i]*(1-p[n]) + (i ? tmp[i-1]*p[n] : 0);
  }
  bt(n+1, k+1);
  r = tmp;
};

int main() {
  cout.setf(ios::fixed);
  cout.precision(8);
  int T; cin >> T;
  for (int t = 1; t <= T; ++t) {
    cin >> N >> K;
    p = vd(N);
    for (double& d : p) cin >> d;
    r = vd(1+K/2, 0);
    r[0] = 1;
    res = 0;
    bt(0, 0);
    cout << "Case #" << t << ": ";
    cout << res << endl;
  }
}