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


const int dxy[8][2] = { {-1,0}, {0,1}, {1,0}, {0,-1}, {-1,1}, {1,1}, {1,-1}, {-1,-1} };


int n, m;
char a[MAXN][MAXN];
int ma1[256], ma2[256];


void read() {
	if (! (cin >> n >> m))
		throw;
	forn(i,n)
		forn(j,m)
			cin >> a[i][j];
}


bool type[MAXN][MAXN];
int cnt[MAXN][MAXN];

bool check() {
	forn(i,n)
		forn(j,m)
			cnt[i][j] = 0;

	forn(i,n)
		forn(j,m) {
			int dir = (type[i][j] ? ma1 : ma2) [a[i][j]];
			int ni = i + dxy[dir][0],
				nj = j + dxy[dir][1];
			ni = (ni + n) % n;
			nj = (nj + m) % m;
			++cnt[ni][nj];
		}

	forn(i,n)
		forn(j,m)
			if (cnt[i][j] != 1)
				return false;
	return true;
}

void solve() {
	int ans = 0;
	forn(msk,1<<(n*m)) {
		forn(i,n)
			forn(j,m)
				type[i][j] = (msk & (1 << (i*m+j))) != 0;

		if (check())
			++ans;
	}
	printf ("%d\n", ans);
}


int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	ma1['|'] = 0;   ma2['|'] = 2;
	ma1['-'] = 3;   ma2['-'] = 1;
	ma1['/'] = 4;   ma2['/'] = 6;
	ma1['\\'] = 7;   ma2['\\'] = 5;

	int ts;
	cin >> ts;
	forn(test,ts) {
		read();
		printf ("Case #%d: ", test+1);
		solve();
	}

}