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
#define sqrt(x) sqrt(abs(x))
#define unq(x) (x.resize(unique(all(x)) - x.begin()))
#define spc(i,n) " \n"[i == n - 1]

using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<string> vs;
typedef double D;
typedef long double LD;
typedef long long ll;
typedef pair<ll, ll> pll;
typedef vector<ll> vll;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int m;
int n;

ll s0, as,cs, rs;
ll m0, am, cm, rm;

vi v[1000500];
int p[1000500];
int val[1000500];
int mm[1000500];

void gen() {
	val[0] = s0;
	p[0] = -1;
	mm[0] = m0;
	for (int i = 1; i < n; i++) {
		ll ns = (val[i - 1] * as + cs) % rs;
		ll nm = (mm[i - 1] * am + cm) % rm;
		val[i] = ns;
		mm[i] = nm;
		p[i] = nm % i;
		v[p[i]].pb(i);
	}

	if (0)
	rep(i, n)
		cout << i << ' ' << p[i] << ' ' << val[i] << endl;
}

int id[1000500];

bool l1(int a, int b) {
	if (val[a] != val[b])
		re val[a] < val[b];
	re a < b;
}

//int cur = 0;

int root[1000500];

int get(int x) {
	if (root[x] == x)
		re x;
	re root[x] = get(root[x]);
}

int col[1000500];

void merge(int a, int b) {
	if (a == b)
		re;

	if (b == 0)
		swap(a, b);

	root[b] = a;
	col[a] += col[b];
}

int gf;

int dd[1000500];

int getans() {
	re col[0];
}

void del(int x) {
	if (dd[x])
		re;
	dd[x] = 1;
	col[get(x)]--;
	rep(i, sz(v[x])) {
		int y = v[x][i];
		del(y);
	}
}

void add(int x) {
	if (x == 0) {
		gf = 1;
		re;
	}

	int a = get(x);
	int b = get(p[x]);

	merge(a, b);
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n >> m;
		rep(i, n)
			v[i].clear();
		cin >> s0 >> as >> cs >> rs;
		cin >> m0 >> am >> cm >> rm;
		gen();

		rep(i, n) {
			id[i] = i;
		}
		sort(id, id + n, l1);

		int ans = 1;
		int r = 0;
		fill(dd, 0);
		gf = 0;

		rep(i, n) {
			root[i] = i;
			col[i] = 1;
		}

		rep(i, n) {
			while (r < n && val[id[r]] <= val[id[i]] + m) {
				add(id[r]);
				r++;
			}
			if (gf)
				ans = max(ans, getans());
			del(id[i]);
		}
		cout << ans << endl;
	}

	re 0;
}
