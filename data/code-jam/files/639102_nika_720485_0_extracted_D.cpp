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
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(i=0;i<n;i++)
#define F1(i,n) for(i=1;i<=n;i++)
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }
const int MOD = 1000000007;
int i, j, k, m, n, l, ans, b, a[1000], zero, live, rem, mask, zero2, live2, rem2;
char ss[1000002];

ll d[101][2][71][71], C[101][101], F[101];

int main() {
//	freopen("d.in", "r", stdin);

	freopen("D-small-attempt0.in", "r", stdin);
	freopen("D-small-attempt0.out", "w", stdout);

//	freopen("D-large.in", "r", stdin);
//	freopen("D-large.out", "w", stdout);

	for (i = 0; i <= 100; i++) {
		C[i][0] = C[i][i] = 1;
		for (j = 1; j < i; j++)
			C[i][j] = (C[i-1][j] + C[i-1][j-1]) % MOD;
		if (i == 0) F[i] = 1;
		else F[i] = (F[i-1] * i) % MOD;
	}

	int tt, tn; cin >> tn;
	F1(tt,tn) {
		cin >> n >> b;
		k = 0;
		while (n > 0) {
			a[k++] = n % b;
			n /= b;
		}

		memset(d, 0, sizeof(d));
		d[0][0][0][b] = 1;

		F0(i,k) {
			F0(zero,2) {
				F0(rem,b) {
					F0(live,b+1) if (d[i][zero][rem][live]) {
						F0(mask,(1<<b)) {
							zero2 = mask&1;
							live2 = 0;
							int sum = 0;
							F0(j,b) if ((1<<j)&mask) {
								sum += j;
								live2++;
							}
							if (live2 > live) continue;
							if ((sum + rem) % b == a[i]) {
								rem2 = (sum + rem) / b;
								if (i > 0) {
									if (zero)
										d[i+1][zero2][rem2][live2] += (d[i][zero][rem][live] * C[live-1][live2-1]) % MOD * F[live2] % MOD;
									else 
										d[i+1][zero2][rem2][live2] += (d[i][zero][rem][live] * C[live][live2]) % MOD * F[live2] % MOD;
								} else {
									d[i+1][zero2][rem2][live2] += 1;
								}
								d[i+1][zero2][rem2][live2] %= MOD;
							}

						}
					}
				}
			}
		}
		ans = 0;
		// i == k
		// zero == 0
		// rem == 0
		// live = any
		F0(i,b+1)
			ans = (ans + d[k][0][0][i]) % MOD;
		printf("Case #%d: ", tt);

		cout << ans << endl;
	}
	
	return 0;
}
