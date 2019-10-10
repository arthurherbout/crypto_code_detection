#include <bits/stdc++.h>
#define inf 0x3f3f3f3f
#define Inf 0x3FFFFFFFFFFFFFFFLL
#define rep(i, n) for (int i = 0; i < (n); ++i)
#define Rep(i, n) for (int i = 1; i <= (n); ++i)
#define FOR(i, c) for(__typeof((c).begin()) i = (c).begin(); i != (c).end(); ++i)
#define clr(x, a) memset(x, (a), sizeof x)
#define RD(x) scanf("%d", &x)
#define PB push_back
#define MP make_pair
using namespace std;
typedef long long ll;
typedef pair<int, int> pii;
typedef vector<int> vi;
int const N = 2020;
ll const mod = 1000000007LL;
char g[N];
int main() {
	freopen("D-small-attempt1.in", "r", stdin);
	freopen("D-small-ans-1", "w", stdout);
	int T, ca = 1; scanf("%d", &T);
	while (T--) {
		int n, l; scanf("%d%d", &n, &l);
		// g
		bool ok = 1;
		rep(i, n) {
			scanf(" %s", g);
			bool fail = 1;
			int len = strlen(g);
			rep(j, len) {
				if (g[j] != '1') fail = 0;
			}
			if (fail) ok = 0;
		}
		// b
		scanf(" %*s");
		printf("Case #%d: ", ca++);
		if (!ok) {
			puts("IMPOSSIBLE");
			continue;
		}
		rep(i, l) printf("0?");
		putchar(' ');
		rep(i, l - 1) printf("1");
		puts("0");
	}
	return 0;
}

