#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(int i=0;i<n;i++)
#define F1(i,n) for(int i=1;i<=n;i++)
const int MOD = 1000002013;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n - 1)&n) + 1 : 0; }
int dx[] = { 0, -1, 0, 1 };
int dy[] = { 1, 0, -1, 0 };
int i, j, k, m, n, l;
ll ans;
ll a[15005], lo[15005], hi[15005], dp[15005], dp2[15005];

int main() {
	//freopen("x.in", "r", stdin);

	freopen("B-small-attempt8.in", "r", stdin);
	freopen("B-small-attempt8.out", "w", stdout);

	//freopen("B-large.in", "r", stdin);
	//freopen("B-large.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt, tn) {
		cin >> n >> k;
		F0(i, n - k + 1) cin >> a[i];

		F0(i, k) {
			ll c = 0;
			lo[i] = hi[i] = 0;
			for (int j = i; j < n - k; j += k) {
				c += a[j + 1] - a[j];
				lo[i] = min(lo[i], c);
				hi[i] = max(hi[i], c);
			}
		}

		ans = 1e18;

		F0(i, k) {
			// lowest 
			F0(j, k) dp[j] = 1e18;
			dp[(-lo[i] % k + k) % k] = hi[i] - lo[i];

			F0(j, k) if (j != i) {
				F0(z, k) dp2[z] = 1e18;
				F0(u, k) {
					// avciet j - u dan
					ll nh = ((u - lo[j]) % k + k) % k + (hi[j] - lo[j]);
					F0(z, k) {
						dp2[(u + z) % k] = min(dp2[(u + z) % k], max(dp[z], nh));
					}
				}
				F0(z, k) dp[z] = dp2[z];
			}
			ans = min(ans, dp[(a[0] % k + k) % k]);
		}
		//cerr << tt << endl;
		printf("Case #%d: ", tt);

		cout << ans << endl;
	}
	return 0;
}
