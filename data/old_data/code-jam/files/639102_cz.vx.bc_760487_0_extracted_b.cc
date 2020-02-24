#include <iostream>
#include <algorithm>
using namespace std;
typedef long long ll;
const ll llinf = 1LL<<50;

ll B[120];
ll D[1001000];
int nb;

int main() {
  int nt, C = 1;
  ll tot;
  cin >> nt;
  while (nt-- && cin >> tot >> nb) {
    for (int i = 0; i < nb; ++i)
      cin >> B[i];

    sort(B, B+nb);

    ll m = 0;  // m < 1000000
    for (int i = 0; i+1 < nb; ++i)
      m += B[i]*(B[i+1]-1);

    for (int i = 0; i <= m; ++i)
      D[i] = llinf;
    D[0] = 0;
    for (int i = 0; i < nb-1; ++i) {
      for (int j = 0; j <= m; ++j)
	D[j+B[i]] = min(D[j+B[i]], D[j]+1);
    }    

    ll ans = llinf;
    for (int i = 0; i <= m; ++i) {
      if (!((tot - i)%B[nb-1]))
	ans = min(ans, D[i] + (tot-i)/B[nb-1]);
    }

    cout << "Case #" << C++ << ": ";
    if (ans < llinf) cout << ans << endl;
    else cout << "IMPOSSIBLE" << endl;
  }
}
