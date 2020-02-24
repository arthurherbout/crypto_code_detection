#include <cstdio>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <algorithm>

using namespace std;

const int N = 55;
char c[N][N];
int work[N], mach[N], cnt[N], d[N], num[N], perm[N];

int find(int x) {
	if (d[x] == x) return x;
	return d[x] = find(d[x]);
}

void run(int cas) {
	int n;
	scanf("%d", &n);
	for (int i = 0; i < n; i++)
		scanf(" %s", c[i]);
	for (int i = 0; i < 2*n; i++)
		d[i] = i;
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (c[i][j] == '1') {
				int x = find(i);
				int y = find(j+n);
				if (x != y)
					d[x] = y;
			}
	int st = 0;
	for (int i = 0; i < 2*n; i++) {
		d[i] = find(i);
		if (d[i] == i) num[i] = st++;
	}
	memset(work, 0, sizeof(work));
	memset(mach, 0, sizeof(mach));
	memset(cnt, 0, sizeof(cnt));
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (c[i][j] == '1' && d[i] == d[j+n])
				cnt[num[d[i]]]++;
	for (int i = 0; i < n; i++) {
		work[num[d[i]]]++;
		mach[num[d[i+n]]]++;
	}
	int ed = 0, ans = n * n, other = 0;
	for (int i = 0; i < st; i++)
		if (work[i] != mach[i])
			perm[ed++] = i;
		else other += (work[i] * mach[i] - cnt[i]);
	int upd = 0;
	do {
		for (int i = 0; i < ed; i++) {
			int k = rand() % (ed - i);
			swap(perm[i], perm[i + k]);
		}
		int wo = 0, ma = 0, lst = 0;
		int cur = 0;
		for (int i = 0; i < ed; i++) {
			wo += work[perm[i]];
			ma += mach[perm[i]];
			if (wo == ma) {
				int tmp = 0;
				for (int j = lst; j <= i; j++)
					tmp += cnt[perm[j]];
				cur += wo * ma - tmp;
				lst = i + 1;
				wo = ma = 0;
			}
		}
		if (cur < ans) {
			ans = cur;
			upd = 0;
		} else upd++;
	} while (upd < 1000000);
	printf("Case #%d: %d\n", cas, ans + other);
}

int main() {
    int tt, cas;
    scanf("%d", &tt);
    for (cas = 1; cas <= tt; cas++)
        run(cas);
    return 0;
}

