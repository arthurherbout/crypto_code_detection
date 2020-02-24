#include <bits/stdc++.h>

using namespace std;

#define error(x) cout << #x << " = " << x << "\n"
#define sz(a) int(a.size())
#define pb push_back

typedef long long int64;
typedef pair<int, int> ii;

const double EPS = 1e-9;
const int MAXN = 105;

struct cell {
	double t;
	int u, horse;
	cell(double _t = 0, int _u = 0, int _horse = 0) : t(_t), u(_u), horse(_horse) {}
	bool operator < (const cell & other) const {
		return t < other.t || (t == other.t && u < other.u) 
			|| (t == other.t && u == other.u && horse < other.horse);
	}
};

set<cell> se;
int N, Q;
int E[MAXN], S[MAXN], D[MAXN][MAXN];
int64 dist[MAXN][MAXN];
double T[MAXN][MAXN];

double opt(double a, double b) {
	if (a == -1) return b;
	return min(a, b);
}
int64 opt(int64 a, int64 b) {
	if (a == -1) return b;
	if (b == -1) return a;
	return min(a, b);
}

double proc(int u, int v) {
	for (int i = 1; i <= N; i++)
		for (int j = 1; j <= N; j++)
			T[i][j] = 1e15;
	T[u][u] = 0;

	se.clear();
	se.insert(cell(0, u, u));
	while (!se.empty()) {
		cell u = *se.begin();
		se.erase(se.begin());
		if (u.u == v) {
			return u.t;
		}
		if (T[u.u][u.u] > T[u.u][u.horse]) {
			set<cell>::iterator it = se.find(cell(T[u.u][u.u], u.u, u.u));
			if (it != se.end()) se.erase(it);
			T[u.u][u.u] = T[u.u][u.horse];
			se.insert(cell(T[u.u][u.u], u.u, u.u));
		}
		int64 used = dist[u.horse][u.u];
		for (int i = 1; i <= N; i++) {
			if (D[u.u][i] == -1) continue;
			if (used + D[u.u][i] > E[u.horse]) continue;
			double t_need = (double)D[u.u][i] / S[u.horse];
			if (T[i][u.horse] > T[u.u][u.horse] + t_need) {
				set<cell>::iterator it = se.find(cell(T[i][u.horse], i, u.horse));
				if (it != se.end()) se.erase(it);
				T[i][u.horse] = T[u.u][u.horse] + t_need;
				se.insert(cell(T[i][u.horse], i, u.horse));
			}
		}
	}
	return -1;
}

void solve() {

	for (int k = 1; k <= N; k++)
		for (int i = 1; i <= N; i++)
			if (dist[i][k] != -1)
				for (int j = 1; j <= N; j++)
					if (dist[k][j] != -1)
						dist[i][j] = opt(dist[i][j], dist[i][k] + dist[k][j]);
	for (int i = 1; i <= N; i++)
		dist[i][i] = 0;

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
		memset(dist, -1, sizeof dist);
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= N; j++) {
				cin >> D[i][j];
				dist[i][j] = D[i][j];
			}
		}
		cout << "Case #" << te << ": ";
		solve();
	}

	return 0;
}