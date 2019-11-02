#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


#define forn(i,n)  for (int i=0; i<int(n); ++i)
#define all(a)  a.begin(), a.end()


const int MAXC = 200010;
const int INF = 1000000000;
const long long INF64 = (long long) 2E18;

int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	for (int tt=1; tt<=ts; ++tt) {

		long long l;
		int n;
		cin >> l >> n;
		vector<int> b (n);
		for (int i=0; i<n; ++i)
			cin >> b[i];

		vector<int> d (MAXC, INF);
		d[0] = 0;
		for (int i=1; i<MAXC; ++i)
			for (int j=0; j<n; ++j)
				if (i-b[j] >= 0)
					d[i] = min (d[i], 1 + d[i-b[j]]);


		long long ans = INF64;

		for (int i=0; i<1000000; ++i) {
			long long x = b[0] * 1ll * i;
			long long y = l - x;
			if (y >= 0) {
				long long j = y / b[1];
				if (y == j * b[1])
					ans = min (ans, i + j);
			}
		}

		/*
		for (int i=0; i<n; ++i) {
			long long c0 = max (0ll, l - MAXC) / b[i];
			while (l - c0 * b[i] < MAXC)
				--c0;
			while (l - c0 * b[i] >= MAXC)
				++c0;
			int curans = d[int(l - c0 * b[i])];
			if (curans != INF)
				ans = min (ans, c0 + curans);

			for (int j=0; j<i; ++j) {
				long long c0 = max (0ll, l - MAXC) / b[i];
				for (int c1=0; c1<=1000000; ++c1) {
					while (l - c0 * b[i] - c1 * b[j] < MAXC)
						--c0;
					while (l - c0 * b[i] - c1 * b[j] >= MAXC)
						++c0;
					if (c0 < 0)
						break;
					int curans = d[int(l - c0 * b[i] - c1 * b[j])];
					if (curans != INF)
						ans = min (ans, c0 + c1 + curans);
				}
			}
		}
		*/

		printf ("Case #%d: ", tt);
		if (ans != INF64)
			printf ("%I64d\n", ans);
		else
			puts ("IMPOSSIBLE");

		cerr << tt << endl;
	}

}

