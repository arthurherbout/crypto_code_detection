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


const int MAXN = 210;
const int MAXV = 2000*1000+10;


int f, m, n, s[MAXN], p[MAXN];


void read() {
	cin >> m >> f >> n;
	forn(i,n)
		cin >> p[i] >> s[i];
}


int cost_here[MAXV], cost[MAXV], d[MAXV];


void solve() {
	forn(i,MAXV)
		cost_here[i] = INF;
	forn(i,n)
		cost_here[s[i]] = min (cost_here[s[i]], p[i]);
	ford(i,MAXV-1)
		cost_here[i] = min (cost_here[i], cost_here[i+1]);

	cost[0] = 0;
	for (int i=1; i<MAXV; ++i)
		cost[i] = min (INF, cost[i-1] + cost_here[i-1]);
	forn(i,MAXV)
		cost[i] += f;
	
	d[0] = 0;
	int iter = 0;
	for (int i=1; i<=m; ++i) {
		int & my = d[i] = d[i-1];

		for (int j=1; j<10; ++j) {
			int c = cost[j];
			if (c >= INF)  continue;
			if (i - c < 0)  continue;
			my = max (my, j + d[i-c]);
		}

		while (iter < MAXV && cost[iter] <= i)
			++iter;
		forn(jj,10) {
			int j = iter-1 - jj;
			if (j <= 0)  break;
			int c = cost[j];
			if (c >= INF)  continue;
			if (i - c < 0)  continue;
			my = max (my, j + d[i-c]);
		}
	}

	int ans = d[m];
	for (int j=1; j<MAXV; ++j) {
		int c = cost[j];
		if (c >= INF)  continue;
		int cnt1 = m / c;
		forn(jj,10) {
			int cnt = cnt1 - jj;
			if (cnt < 0)  break;
			ans = max (ans, cnt * j + d[m - cnt * c]);
		}
	}

	cout << ans << endl;
}


int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	forn(tt,ts) {
		cerr << tt+1 << endl;
		read();
		if (! cin)  throw;
		printf ("Case #%d: ", tt+1);
		solve();
	}
}