#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <deque>
#include <cstdint>

using namespace std;

typedef int64_t ll;

ll reverse(ll n){
	ll r = n%10;
	while (n / 10 > 0) {
		r *= 10;
		n /= 10;
		r += n % 10;
	}
	return r;

}

ll A[1000005];

ll F(ll n) {
	
	if (A[n] != -1)
		return A[n];
	if (n == 1) {
		A[1] = 1;
		return 1;
	}
	ll n1 = reverse(n);
	ll  r;
	if (n1 < n && n % 10 != 0) {
		r = min(F(n - 1), F(n1));
	}
	else {
		r = F(n - 1);
	}
	++r;
	A[n] = r;
	return r;
}

void main() {
	int T;
	cin >> T;
	for (int i = 0;i < 1000004;++i) {
		A[i] = -1;
	}
	for (int i = 1;i < 1000000;++i) {
		A[i] = F(i);
	}
	for (int i = 0; i < T; ++i) {
		ll n;
		ll r=0;
		cin >> n;
		ll n2 = n;
		ll r2 = F(n2);
		while (n >= 1) {
			int d = n % 10;
			if (d == 0) {
				--n;
				++r;
			}
			else if (d > 1) {
				n -= d - 1;
				r += d - 1;
			}
			else if (d == 1) {
				if (reverse(n) < n) {
					n = reverse(n);
					++r;
				}
				else {
					r += n / 10 + 1;
					n -= n / 10 +1;
				}
			}
		}
		cout << "Case #" << i + 1 << ": " << r2 << endl;
	}
}