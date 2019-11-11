#include <iostream>
#include <cmath>
#include <algorithm>
#include <numeric>
#include <string>
#include <vector>
#include <queue>
#include <set>
#include <bitset>
#include <deque>
#include <map>
#include <stack>
#include <sstream>
#include <cstring>

#define mp make_pair
#define pb push_back
#define all(x) (x).begin(), (x).end()
#define sz(x) ((int)((x).size()))
#define sqr(x) ((x)*(x))
#define For(i,a,b) for(int i = (a); i < (b); i++)
#define rep(i,n) For(i,0,n)
#define re return
#define fi first
#define se second
#define y0 y47847892
#define y1 y47824262
#define fill(x, val) memset(x, val, sizeof(x))

using namespace std;

typedef vector<int> vi;
typedef long long ll;
typedef vector<string> vs;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef vector<vi> vvi;

template<class T> T abs(T x) { return x > 0 ? x : -x;}

int n;
int m;

double dist(double x1, double y1, double x2, double y2) {
	re sqrt(sqr(x1 - x2) + sqr(y1 - y2));
}

int main() {

#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif

	int tCount;
	cin >> tCount;
	rep(test, tCount) {
		cout << "Case #" << test + 1 << ":" << ' ' ;

		cin >> n;
		int x[10], y[10], r[10];
		rep(i, n)
			cin >> x[i] >> y[i] >> r[i];

		double ans = 1000000;
		rep(i1, n) rep(j1, n) rep(i2, n) rep(j2, n) {
			int was[10];
			memset(was, 0, sizeof(was));
			was[i1] = was[j1] = was[i2] = was[j2] = 1;
			int z = 1;
			rep(i, n)
			if (!was[i]) {
				z = 0;
			}
			if (z == 0)
				continue;
			double r1 = dist(x[i1], y[i1], x[j1], y[j1]) + r[i1] + r[j1];
			double r2 = dist(x[i2], y[i2], x[j2], y[j2]) + r[i2] + r[j2];

			ans = min(ans, max(r1, r2));
		}

		printf("%.5lf", ans / 2);
		cout << endl;
	}

	return 0;
}
