// Google Code Jam 2016 Round 1B
// Problem C. Technobabble

#include <iostream>
#include <sstream>
#include <algorithm>
#include <set>
#include <map>
#include <vector>

using namespace std;

typedef long long LL;

static inline int popcount(unsigned int b)
{
#ifdef __GNUC__
	return __builtin_popcount(b);
#elif _MSC_VER >= 1400
	return __popcnt(b);
#else
	b = (b & 0x55555555) + (b >> 1 & 0x55555555);
	b = (b & 0x33333333) + (b >> 2 & 0x33333333);
	b = (b & 0x0F0F0F0F) + (b >> 4 & 0x0F0F0F0F);
	b += b >> 8;
	b += b >> 16;
	return b & 0x3F;
#endif
}

LL solve() {
	int N;
	cin >> N;
	map<string, int> da, db;
	int ai = 0, bi = 0;
	int af = 0, bf = 0;
	vector<int> va(N), vb(N);
	for (int i = 0; i < N; ++i) {
		string a, b;
		cin >> a >> b;
		int p = da[a];
		if (p <= 0) {
			p = ++ai;
			da[a] = p;
			af |= (1 << p);
		}
		int q = db[b];
		if (q <= 0) {
			q = ++bi;
			db[b] = q;
			bf |= (1 << q);
		}
		va[i] = (1 << p);
		vb[i] = (1 << q);
	}
	int ans = 0;
	int mx = 1 << N;
	for (int i = 1; i < mx; ++i) {
		int x = 0, y = 0;
		for (int j = 0; j < N; ++j) {
			if ((1 << j) & i) {
				x |= va[j];
				y |= vb[j];
			}
		}
		if (x == af && y == bf) {
			ans = max(ans, N - popcount(i));
		}
	}
	return ans;
}

int main(int argc, char *argv[]) {
	LL T;
	cin >> T;
	for (int t = 1; t <= T; ++t) {
		LL ans = solve();
		cout << "Case #" << t << ": " << ans << endl;
	}
	return 0;
}
