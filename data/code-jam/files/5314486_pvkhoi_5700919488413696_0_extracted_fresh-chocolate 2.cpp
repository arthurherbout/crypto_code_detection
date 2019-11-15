#include <bits/stdc++.h>

using namespace std;

#define error(x) cout << #x << " = " << x << endl
#define pb push_back
#define sz(a) int(a.size())

typedef long long int64;
typedef pair<int, int> ii;

int N, P, G[105], cnt[5];
int f[105][105][105];

int optimize(int a, int b) {
	if (b == -1) return a;
	if (a == -1) return b;
	return max(a, b);
}

int solve() {
	memset(cnt, 0, sizeof cnt);
	for (int i = 0; i < N; i++)
		cnt[G[i]]++;
	int res = cnt[0];

	// error(cnt[1]);
	// error(cnt[2]);
	// error(cnt[3]);

	memset(f, -1, sizeof f);
	f[0][0][0] = 0;
	for (int i = 0; i <= cnt[1]; i++)
		for (int j = 0; j <= cnt[2]; j++)
			for (int k = 0; k <= cnt[3]; k++) {
				if (f[i][j][k] == -1) continue;
				// error(i); error(j); error(k);
				// error(f[i][j][k]);
				int s = (i + j*2 + k*3) % P;
				int add = (s == 0) ? 1 : 0;
				if (i < cnt[1])
					f[i+1][j][k] = optimize(f[i][j][k] + add, f[i+1][j][k]);
				if (j < cnt[2])
					f[i][j+1][k] = optimize(f[i][j][k] + add, f[i][j+1][k]);
				if (k < cnt[3])
					f[i][j][k+1] = optimize(f[i][j][k] + add, f[i][j][k+1]);
			}

	return res + f[cnt[1]][cnt[2]][cnt[3]];
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios::sync_with_stdio(0);
	int T; cin >> T;
	for (int te = 1; te <= T; te++) {
		cin >> N >> P;
		for (int i = 0; i < N; i++) {
			cin >> G[i];
			G[i] %= P;
		}
		cout << "Case #" << te << ": " << solve() << "\n";
	}

	return 0;
}