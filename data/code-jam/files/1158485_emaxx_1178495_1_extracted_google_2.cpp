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


const int MAXN = 1100;
const int MAXC = 10010;


int n;
int a[MAXN];


void read() {
	if (! (cin >> n))
		throw;
	forn(i,n) {
		cin >> a[i];
	}
	sort (a, a+n);
}


int have[MAXC+10], b[MAXC+10];


bool can (int len) {
	forn(i,MAXC)
		have[i] = 0;
	forn(i,MAXC)
		b[i] = 0;
	forn(i,n)
		++b[a[i]];

	forn(i,MAXC) {
		while (b[i]) {
			if (have[i] > 0) {
				++have[i+1];
				--b[i];
				--have[i];
				continue;
			}

			forn(j,len) {
				int idx = i + j;
				if (b[idx] == 0)
					return false;
				--b[idx];
			}
			++have[i+len];
		}
	}
	return true;
}


void solve() {
	if (n == 0) {
		puts ("0");
		return;
	}

	int l = 1,  r = n;
	while (l < r) {
		int m = (l + r + 1) / 2;
		if (can (m))
			l = m;
		else
			r = m - 1;
	}

	int ans = l;
	if (!can (ans))
		ans = 0;
	printf ("%d\n", ans);
}


int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	forn(test,ts) {
		read();
		printf ("Case #%d: ", test+1);
		solve();
	}

}