#include <iostream>
#include <vector>
using namespace std;

typedef long long int ll;
typedef vector<int> vi;

inline ll hm(const vi& M, ll T) {
  ll res = 0;
  for (int m : M) res += 1 + T/m;
  return res;
}

inline int who() {
  int B, N; cin >> B >> N;
  vi M(B); for (int& x : M) cin >> x;
  if (N <= B) return N;
  ll T = 1;
  while (hm(M, T) < N) T *= 2;
  ll L = T/2, R = T;
  while (R-L > 1) {
    T = (L+R)/2;
    if (hm(M, T) < N) L = T;
    else R = T;
  }
  if (N-hm(M, L) > B) cerr << "ERROR\n";
  return N-hm(M, L);
}

int main() {
  int T; cin >> T;
  for (int cas = 1; cas <= T; ++cas) {
    cout << "Case #" << cas << ": " << who() << endl;
  }
}
