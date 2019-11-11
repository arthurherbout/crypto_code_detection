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
		for (int i=0; i<n; ++i) {
			long long c0 = max (0ll, l - MAXC) / b[i];
			while (l - c0 * b[i] >= MAXC)
				++c0;
			int curans = d[int(l - c0 * b[i])];
			if (curans != INF)
				ans = min (ans, c0 + curans);
		}

		printf ("Case #%d: ", tt);
		if (ans != INF64)
			printf ("%I64d\n", ans);
		else
			puts ("IMPOSSIBLE");
	}

}

