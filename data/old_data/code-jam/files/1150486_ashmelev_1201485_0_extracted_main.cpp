#ifndef LOCAL_BOBER
#pragma comment(linker, "/STACK:134217728")
#endif

#include <iostream>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <bitset>
#include <queue>
#include <stack>
#include <sstream>
#include <cstring>
#include <numeric>
#include <ctime>

#define re return
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int) (x).size())
#define rep(i, n) for (int i = 0; i < (n); i++)
#define rrep(i, n) for (int i = (n) - 1; i >= 0; i--)
#define y0 y32479
#define y1 y95874
#define fill(x, y) memset(x, y, sizeof(x))
#define sqr(x) ((x) * (x))
#define prev prev239
#define next next239
#define hash hash239
#define rank rank239
#define sqrt(x) sqrt(abs(x))

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef long long ll;
typedef double D;
typedef long double LD;

template<class T> T abs(T x) {return x > 0 ? x : -x;}

int n;
int m;

int d[500];
vi v[500];

vi vv;
queue<int> q;

void parse(int x) {
	int dist = d[x];
	rep(i, sz(v[x])) {
		int u = v[x][i];
		if (d[u] == -1) {
			q.push(u);
			d[u] = dist + 1;
		}
	}
}

int ans[1000];
int col[1000];

int matr[500][500];

void calc(int x) {
	ans[x] = 1000000;
	rep(i, sz(v[x])) {
		int u = v[x][i];
		if (d[u] < d[x]) {
			int tmp = ans[u];
			rep(j, n)
			if (d[j] == d[u])
			if (u != j && ((matr[j][u] && u != 1) || matr[j][x]))
				tmp++;
			ans[x] = min(ans[x], tmp);
		}
	}
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tc;
	cin >> tc;
	rep(tt, tc) {
		printf("Case #%d: ", tt + 1);

		cin >> n >> m;
		rep(i, n)
		v[i].clear();
		fill(matr, 0);
		rep(i, m) {
			string s;
			cin >> s;
			replace(all(s), ',', ' ');
			stringstream sin(s);
			int a, b;
			sin >> a >> b;
			matr[a][b] = matr[b][a] = 1;
			v[a].pb(b);
			v[b].pb(a);
		}

		fill(d, -1);
		q.push(1);
		d[1] = 0;
		vv.clear();
		while (!q.empty()) {
			parse(q.front());
			vv.pb(q.front());
			q.pop();
		}

		int o = sz(v[0]);

		if (d[0] == 1) {
			cout << 0 << ' ' << o << endl;
			continue;
		}

		o = 0;
		rep(i, sz(v[0])) {
			int u = v[0][i];
			if (d[u] != d[0] - 1)
				o++;
		}

		ans[1] = 1;
		for (int i = 1; i < sz(vv); i++)
			calc(vv[i]);

		cout << d[0] - 1 << ' ' << ans[0] + o << endl;
	}

	re 0;
}

