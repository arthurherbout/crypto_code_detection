#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

const int N = 1000010;
int a[N];
long long s[N];

void run(int cas) {
	int n, p, q, r, rem;
	scanf("%d%d%d%d%d", &n, &p, &q, &r, &rem);
	for (int i = 1; i <= n; i++) {
		a[i] = ((i - 1) * (long long)p + q) % r + rem;
		s[i] = s[i - 1] + a[i];
	}
	int ll = 1, rr = 1;
	long long ans = s[n];
	while (ll <= n) {
		long long lef = s[ll - 1];
		while (rr < n) {
			long long mid = s[rr + 1] - s[ll - 1];
			long long rig = s[n] - s[rr + 1];
			if (mid > rig) break;
			rr += 1;
		}
		long long mid = s[rr] - s[ll - 1];
		long long rig = s[n] - s[rr];
		long long mini = max(lef, max(mid, rig));
		ans = min(ans, mini);
		if (rr < n) {
			mid = s[rr + 1] - s[ll - 1];
			rig = s[n] - s[rr + 1];
			mini = max(lef, max(mid, rig));
			ans = min(ans, mini);
		}
		ll += 1;
	}
	printf("Case #%d: %.12f\n", cas, (double)(s[n] - ans) / s[n]);
}

int main() {
    int tt, cas;
    scanf("%d", &tt);
    for (cas = 1; cas <= tt; cas++)
        run(cas);
    return 0;
}
