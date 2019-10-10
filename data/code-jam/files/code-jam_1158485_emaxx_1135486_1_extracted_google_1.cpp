#pragma comment (linker, "/STACK:200000000")
#define _CRT_SECURE_NO_DEPRECATE
#define _SECURE_SCL 0
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;


typedef long long int64;
typedef double old_double;
#define double long double
const int INF = (int) 1E9;
const int64 INF64 = (int64) 1E18;
const double EPS = 1E-9;
const double PI = acos((double)0) * 2;

#define forn(i,n)  for (int i=0; i<int(n); ++i)
#define ford(i,n)  for (int i=int(n)-1; i>=0; --i)
#define fore(i,l,n)  for (int i=int(l); i<int(n); ++i)
#define all(a)  a.begin(), a.end()
#define fs  first
#define sc  second
#define pb  push_back
#define mp  make_pair


const int MAXN = 110;
const int MAXC = 1010;


struct pt {
	double x, y;
	
	bool operator< (const pt & p) const {
		return x < p.x - EPS;
	}
};


int need;
double w;
int n1, n2;
vector<pt> up, down;


void read() {
	if (! (cin >> w >> n1 >> n2 >> need))
		throw;
	down.resize (n1);
	forn(i,n1) {
		cin >> down[i].x >> down[i].y;
		down[i].y += MAXC;
	}
	up.resize (n2);
	forn(i,n2) {
		cin >> up[i].x >> up[i].y;
		up[i].y += MAXC;
	}
}


double sup[MAXN], sdown[MAXN];

void build (vector<pt> v, double s[]) {
	int n = (int) v.size();

	double curs = 0;
	forn(i,n) {
		if (i)
			curs += (v[i].y + v[i-1].y) / 2 * (v[i].x - v[i-1].x);
		s[i] = curs;
	}
}

double get_y (pt a, pt b, double x) {
	return a.y + (b.y - a.y) * (x - a.x) / (b.x - a.x);
}

double get_s (vector<pt> & v, double s[], double x) {
	int n = (int)v.size();
	
	pt p = { x, 0 };
	int i = int (lower_bound (all(v), p) - v.begin());
	if (i == 0)  return 0;

	double res = s[i-1];
	res += (v[i-1].y + get_y (v[i-1], v[i], x)) * (x - v[i-1].x) / 2;
	return res;
}

double get_s (double x) {
	return get_s (up, sup, x) - get_s (down, sdown, x);
}

double solve (double s) {
	double l = 0,  r = w;
	forn(it,100) {
		double m = (l + r) / 2;
		if (get_s (m) > s)
			r = m;
		else
			l = m;
	}
	return (l + r) / 2;
}

void solve() {
	build (up, sup);
	build (down, sdown);

	double total = sup[up.size()-1] - sdown[down.size()-1];
	fore(i,1,need) {
		double ans = solve (total * i / need);
		printf ("%.20lf\n", (old_double)ans);
	}
}


int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	forn(test,ts) {
		read();
		printf ("Case #%d:\n", test+1);
		solve();
	}

}