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

long double cnk[5000][5000];

double get(int n, int k) {
	re cnk[n][k];
	/*
	double ans = 1;
	rep(i, n)
		ans /= 2;
	for (int i = n; i > n - k; i--)
		ans *= i;
	for (int i = 1; i <= k; i++)
		ans /= i;
	//cout << n << ' ' << k << ' ' << ans << endl;
	re ans;*/
}

double getans(int x, int y) {
	int z = 1;
	int p = 0;
	while (n >= z) {
		n -= z;
		p += 2;
		z += 4;
	}

	x = abs(x);

	//cout << n << ' ' << p << endl;

	if (x + y < p)
		re 1;

	if (x + y > p)
		re 0;

	if (y == p)
		re 0;

	if (n - p > y)
		re 1;
	double ans = 0;
	for (int o = y + 1; o <= n; o++)
		ans += get(n, o);
	re ans;
}

int main() {
#ifdef LOCAL_BOBER
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tc;
	cin >> tc;

	cnk[0][0] = 1;
	for (int i = 1; i < 5000; i++) {
		cnk[i][0] = cnk[i - 1][0] / 2;
		for (int j = 0; j < 5000; j++)
			cnk[i][j] = (cnk[i - 1][j] + cnk[i - 1][j - 1]) / 2;
	}

	rep(tt, tc) {
		cout << "Case #" << tt + 1 << ": ";

		int x, y;
		cin >> n >> x >> y;

		printf("%.6lf", getans(x, y));
		cout << endl;
	}


	re 0;
}
