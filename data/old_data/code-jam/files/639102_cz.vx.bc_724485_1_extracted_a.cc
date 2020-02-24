#include <iostream>
#include <set>
#include <cstring>
#include <cmath>
using namespace std;
typedef long long ll;

const int MAXP = 1000000;

char ispr[MAXP+1];
ll pr[MAXP], npr;
ll K[20];
ll T[20];

ll modpow(ll a, ll p, ll M) {
    ll r;
    for (r = 1; p; a = (a*a)%M, p /= 2) if (p%2) r = (r*a)%M;
    return r;
}

ll modinv(ll a, ll P) {
  return modpow(a, P-2, P);
}

int main() {
  memset(ispr, 1, sizeof ispr);
  ispr[0] = ispr[1] = 0;
  npr = 0;
  for (int i = 2; i < MAXP; ++i) {
    if (ispr[i]) {
      pr[npr++] = i;
      if (i < sqrt(MAXP))
	for (int j = i*i; j < MAXP; j += i)
	  ispr[j] = 0;
    }
  }

  T[0] = 1;
  for (int i = 1; i < 10; ++i)
    T[i] = T[i-1]*10;

  int nt, nd, nk, C = 1;
  cin >> nt;
  while (nt-- && cin >> nd >> nk) {
    ll maxk = 0;
    for (int i = 0; i < nk; ++i)
      cin >> K[i], maxk = max(maxk, K[i]);
    
    ll ans = -1;
    for (int i = 0; i+1 < nk; ++i)
      if (K[i] == K[nk-1])
	ans = K[i+1];

    if (ans == -1 && nk > 2) {
      set<ll> A;

      ll lim = T[nd];
      for (int i = 0; i < npr && pr[i] <= lim; ++i)
	if (pr[i] > maxk) {
	  ll p = pr[i];
	  ll a = (((K[2]-K[1]+p)%p)*modinv((K[1]-K[0]+p)%p, p))%p;
	  ll b = (K[1]-((a*K[0])%p)+p)%p;

	  //cout << p << ' ' << a << ' ' << b << endl;
	  
	  bool f = true;
	  for (int i = 0; i+1 < nk && f; ++i)
	    if (((a*K[i])%p+b)%p != K[i+1])
	      f = false;

	  if (f) {
	    //cout << a << ' ' << b << ' ' << p << endl;
	    //cout << ((a*K[nk-1])%p+b)%p << endl;
	    A.insert(((a*K[nk-1])%p+b)%p);
	  }
	}

      if (A.size() == 1)
	ans = *A.begin();
    }

    cout << "Case #" << C++ << ": ";
    if (ans == -1) cout << "I don't know." << endl;
    else cout << ans << endl;
  }
}
