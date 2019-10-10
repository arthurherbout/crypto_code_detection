#define _CRT_SECURE_NO_DEPRECATE
#define _SECURE_SCL 0
#pragma comment (linker, "/STACK:200000000")
#include <algorithm>
#include <bitset>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <stack>
#include <sstream>
#include <vector>

using namespace std;

typedef long long int64;
//typedef double old_double;
//#define double long double
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


const int MAXN = 2100;


int n, mx[MAXN];


void read() {
	cin >> n;
	forn(i,n-1) {
		scanf ("%d", &mx[i]);
		--mx[i];
	}
}


bool no_solution;
int ans[MAXN];


double get_y (int x1, int y1, int x2, int y2, int x) {
	if (x1 == x2)  throw;
	return y1 + (y2 - y1) * 1.0 * (x - x1) / (x2 - x1);
}

int get_lower (int x1, int y1, int x2, int y2, int x) {
	double y = get_y (x1, y1, x2, y2, x);
	if (y < -EPS)
		throw;
	
	int iy = min ((int) y, INF);
	if (abs (y - iy) < EPS) {
		if (iy - 1 < 0)  throw;
		return iy - 1;
	}
	if (iy < 0)  throw;
	return iy;
}


void rec (int l, int r, int x1, int y1, int x2, int y2) {
	if (l > r)
		return;

	if (l == n-1) {
		if (ans[l] == -1)
			ans[l] = get_lower (x1, y1, x2, y2, l);
		return;
	}

	if (mx[l] > r+1)
		no_solution = true;
	if (no_solution)
		return;

	if (ans[mx[l]] != -1) {
		if (ans[l] == -1)
			if (mx[l] != x2)
				ans[l] = get_lower (mx[l], ans[mx[l]], x2, y2, l);
			else
				ans[l] = get_lower (x1, y1, x2, y2, l);
	}
	else {
		ans[mx[l]] = get_lower (x1, y1, x2, y2, mx[l]);
		if (x2 <= r)
			--ans[mx[l]];
	}

	if (ans[l] == -1)
		if (x2 >= r)
			ans[l] = get_lower (mx[l], ans[mx[l]], x2, y2, l);
		else
			ans[l] = get_lower (x1, y1, x2, y2, l);

	rec (l+1, mx[l]-1, l, ans[l], mx[l], ans[mx[l]]);
	rec (mx[l], r, l, ans[l], mx[l], ans[mx[l]]);
}


bool check() {
	forn(i,n-1) {
		for (int j=i+1; j<mx[i]; ++j)
			if (ans[j] >= get_y (i, ans[i], mx[i], ans[mx[i]], j) - EPS)
				return false;
		for (int j=mx[i]+1; j<n; ++j)
			if (ans[j] > get_y (i, ans[i], mx[i], ans[mx[i]], j) + EPS)
				return false;
	}
	return true;
}


int my_rand() {
	int x = rand() + rand() * RAND_MAX;
	x %= INF+1;
	return abs(x);
}

void tupo() {
	for(;;) {
		forn(i,n)
			ans[i] = my_rand();
		if (check())
			break;
	}
}


void solve() {
	mx[n-1] = -1;
	forn(i,n)
		ans[i] = -1;
	ans[0] = ans[mx[0]] = INF;
	no_solution = false;
	rec (1, mx[0]-1, 0, ans[0], mx[0], ans[mx[0]]);
	rec (mx[0], n-1, 0, ans[0], mx[0], ans[mx[0]]);

	if (no_solution)
		puts (" Impossible");
	else {
		if (!check())
			tupo();

		forn(i,n)
			printf (" %d", ans[i]);
		puts("");
	}
}


int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	forn(tt,ts) {
		read();
		if (! cin)  throw;
		printf ("Case #%d:", tt+1);
		solve();
	}
}