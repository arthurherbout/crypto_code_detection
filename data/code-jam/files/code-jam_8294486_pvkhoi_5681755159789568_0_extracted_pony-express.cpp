#include <bits/stdc++.h>

using namespace std;

#define error(x) cout << #x << " = " << x << "\n"
#define sz(a) int(a.size())
#define pb push_back

typedef long long int64;
typedef pair<int64, int64> ii;

const double EPS = 1e-9;
const int MAXN = 105;

int N, Q;
int E[MAXN], S[MAXN], D[MAXN][MAXN];
double f[MAXN][MAXN];
int64 P[MAXN];

double opt(double a, double b) {
	if (a == -1) return b;
	return min(a, b);
}

double proc(int u, int v) {
	for (int i = 2; i <= N; i++) {
		P[i] = P[i-1] + D[i-1][i];
	}

	for (int i = 0; i < MAXN; i++)
		for (int j = 0; j < MAXN; j++)
			f[i][j] = -1;

	f[1][1] = 0;
	for (int i = 1; i < N; i++) {
		for (int j = 1; j <= i; j++) {
			if (f[i][j] == -1) continue;
			// error(i); error(j); error(D[i][i+1]); error(S[j]);
			int64 used = P[i] - P[j];
			if (used + D[i][i+1] <= E[j]) {
				f[i+1][j] = opt(f[i+1][j], f[i][j] + (double)D[i][i+1] / S[j]);
			}
			if (D[i][i+1] <= E[i]) {
				f[i+1][i] = opt(f[i+1][i], f[i][j] + (double)D[i][i+1] / S[i]);
			}
		}
	}

	// error(f[1][1]);
	// error(f[2][1]);
	// error(f[3][1]);

	double res = 1e15;
	for (int i = 1; i <= N; i++) {
		if (f[N][i] == -1) continue;
		res = min(res, f[N][i]);
		// error(f[N][i]);
	}
	return res;
}

void solve() {
	vector<double> res;
	while (Q--) {
		int u, v; cin >> u >> v;
		res.pb(proc(u, v));
	}
	for (int i = 0; i < sz(res)-1; i++)
		cout << fixed << setprecision(6) << res[i] << " ";
	cout << fixed << setprecision(6) << res[sz(res)-1] << "\n";
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios::sync_with_stdio(0);
	int T; cin >> T;
	for (int te = 1; te <= T; te++) {
		cin >> N >> Q;
		for (int i = 1; i <= N; i++) {
			cin >> E[i] >> S[i];
		}
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++)
				cin >> D[i][j];
		}
		cout << "Case #" << te << ": ";
		solve();
	}

	return 0;
}