#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


#define forn(i,n)  for (int i=0; i<int(n); ++i)
#define all(a)  a.begin(), a.end()


const int MAXN = 1000001;
bool pr[MAXN];

int powmod (int a, int b, int p) {
	int res = 1;
	while (b)
		if (b & 1)
			--b,  res = res * 1ll * a % p;
		else
			b >>= 1,  a = a * 1ll * a % p;
	return res;
}

int rev (int a, int p) {
	return powmod (a, p-2, p);
}

int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	memset (pr, 1, sizeof pr);
	for (int i=2; i<MAXN; ++i)
		if (pr[i])
			for (int j=i+i; j<MAXN; j+=i)
				pr[j] = false;

	int ts;
	cin >> ts;
	for (int tt=1; tt<=ts; ++tt) {

		int d, k;
		cin >> d >> k;
		
		int maxnum = 1;
		for (int i=0; i<d; ++i)
			maxnum *= 10;

		vector<int> s (k);
		for (int i=0; i<k; ++i)
			cin >> s[i];

		printf ("Case #%d: ", tt);

		if (k == 1) {
			puts ("I don't know.");
			continue;
		}

		if (s[0] == s[1]) {
			for (int i=2; i<k; ++i)
				if (s[i] != s[0])
					throw;
			printf ("%d\n", s[0]);
			continue;
		}

		int ans = -1;
		int cnt = 0;

		if (k == 2) {

			for (int i=s[0]+1; i<=maxnum; ++i)
				if (pr[i])
					++cnt;
			
			if (cnt > 10) {
				puts ("I don't know.");
				continue;
			}

			cnt = 0;
			for (int p=max(2,s[0]+1); p<=maxnum; ++p)
				if (pr[p]) {
					for (int a=0; a<p; ++a) {
						int s1 = (s[0] * 1ll * a) % p;
						int b = (s[1] - s1 + p) % p;
						int curans = ((s[k-1] * 1ll * a + b) % p + p) % p;
						if (ans != curans) {
							ans = curans;
							++cnt;
						}
					}
				}

			if (cnt == 0)
				throw;
			else if (cnt > 1)
				puts ("I don't know.");
			else
				printf ("%d\n", ans);

			continue;

		}

		for (int p=max(2,s[0]+1); p<=maxnum; ++p)
			if (pr[p]) {
				int a =
					(((s[1] - s[2]) % p) + p) % p * 1ll
					*
					rev ((((s[0] - s[1]) % p) + p) % p, p)
					% p;
				int b = ((s[1] - a * 1ll * s[0]) % p + p) % p;
				bool ok = true;
				for (int i=1; i<k; ++i) {
					int num = ((s[i-1] * 1ll * a + b) % p + p) % p;
					if (num != s[i]) {
						ok = false;
						break;
					}
				}
				if (ok) {
					int curans = ((s[k-1] * 1ll * a + b) % p + p) % p;
					if (ans != curans) {
						ans = curans;
						++cnt;
					}
				}
			}

		if (cnt == 0)
			throw;
		else if (cnt > 1)
			puts ("I don't know.");
		else
			printf ("%d\n", ans);

	}

}

