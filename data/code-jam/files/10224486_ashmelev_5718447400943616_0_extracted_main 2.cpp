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
#define next next239
#define prev prev239

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

template<class T> string toString(T x) {
	stringstream sout;
	sout << x;
	re sout.str();
}

int nint() {
	int x;
	scanf("%d", &x);
	re x;
}

int m;
int n;
int ans;

int a[10][10];
int b[10][10];
int cur[10];

int pp[10];

int go(int p) {
	if (p == n)
		re 1;

	int x = pp[p];
	int f = 0;
	rep(i, n) {
		if (b[x][i] && !cur[i]) {
			f = 1;
			cur[i] = 1;
			if (!go(p + 1))
				re 0;
			cur[i] = 0;
		}
	}

	if (!f)
		re 0;

	re 1;
}

int check() {
	rep(i, n)
		pp[i] = i;
	do {
		fill(cur, 0);
		if (!go(0))
			re 0;
	}
	while (next_permutation(pp, pp + n));
	re 1;
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#else
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif


	int tc;
	cin >> tc;
	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		cin >> n;
		rep(i, n) {
			string s;
			cin >> s;
			rep(j, n)
				a[i][j] = s[j] - '0';
		}

		ans = n * n;
		rep(o, (1 << (n * n))) {

			rep(i, n) rep(j, n)
				b[i][j] = ((o >> (i * n + j)) & 1);

			int g = 1;
			int c = 0;
			rep(i, n) rep(j, n) {
				if (a[i][j] && !b[i][j])
					g = 0;
				if (b[i][j] && !a[i][j])
					c++;
			}
			if (!g)
				continue;


			if (!check())
				continue;

			ans = min(ans, c);

		}
		cout << ans << endl;
	}


	re 0;
}










