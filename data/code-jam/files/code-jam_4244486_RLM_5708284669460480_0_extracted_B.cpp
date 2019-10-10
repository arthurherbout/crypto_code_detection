#include <iostream>
#include <vector>
using namespace std;

#define eps 1e-8

typedef vector<int> vi;
typedef long double ld;
typedef vector<ld> vd;
typedef vector<vd> vvd;
typedef vector<bool> vb;

vi build_kmp(const string& W) {
  int L = W.length();
  vi T(L+1);
  T[0] = -1;
  T[1] = 0;
  int pos = 2, cnd = 0;
  while (pos <= L) {
    if (W[pos-1] == W[cnd]) T[pos++] = ++cnd;
    else if (cnd) cnd = T[cnd];
    else T[pos++] = 0;
  }
  return T;
}

ld solve() {
  int K, L, S; cin >> K >> L >> S;
  vi m(26, 0);
  for (int i = 0; i < K; ++i) {
    char c; cin >> c; ++m[c-'A'];
  }
  string target; cin >> target;
  for (char c : target) if (!m[c-'A']) return 0;
  vi T = build_kmp(target);
  int max = 1 + (S-L)/(L-T[L]);
  ld exp = 0;
  vvd P(S+1, vd(L+1, 0));
  P[0][0] = 1;
  for (int i = 0; i < S; ++i) {
    for (int j = 0; j <= L; ++j) {
      int k = j;
      if (j == L) {
        exp += P[i][j];
        k = T[k];
      }
      ld p = 1;
      vb used(26, 0);
      while (k >= 0) {
        if (!used[target[k]-'A']) {
          ld q = m[target[k]-'A']/(double)K;
          P[i+1][k+1] += P[i][j]*q;
          p -= q;
        }
        used[target[k]-'A'] = 1;
        k = T[k];
      }
      P[i+1][0] += P[i][j]*p;
    }
  }
  exp += P[S][L];
#ifdef DBG
  for (vd v : P) { for (ld p : v) cout << p << ' '; cout << endl; }
#endif
  return max - exp;
}

int main() {
  cout.setf(ios::fixed);
  cout.precision(8);
  int T; cin >> T;
  for (int test = 1; test <= T; ++test) {
    cout << "Case #" << test << ": " << solve() << endl;
  }
}
