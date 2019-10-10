#include <iostream>
using namespace std;
typedef long long ll;

ll isect(ll a0, ll a1, ll b0, ll b1) {
  if (a1 < b0) return 0;
  if (a0 > b1) return 0;
  return min(a1, b1) - max(a0, b0) + 1;
}

bool w(ll a, ll b) {
  if (b <= a/2) return true;
  if (a >= (a-b)*3) return false;
  return !w(b, a-b);
}

ll fl(ll a) {
  ll lo = a/2, hi = a;

  while (hi-lo > 1) {
    ll md = (lo+hi)/2;
    if (w(a, md)) lo = md;
    else hi = md;
  }
  return hi;
}

int main() {
  int nt, C = 1;
  cin >> nt;
  ll a0, a1, b0, b1;
  while (nt-- && cin >> a0 >> a1 >> b0 >> b1) {
    ll wins = 0;
    if (b1-b0 < a1-a0) swap(a0, b0), swap(a1, b1);
    for (ll a = a0; a <= a1; ++a) {
      ll c = fl(a);
      wins += isect(b0, b1, 1, c-1) + isect(b0, b1, a+c, a+a) + isect(b0, b1, a+a+1, b1);
    }

    cout << "Case #" << C++ << ": " << wins << endl;
  }
}
