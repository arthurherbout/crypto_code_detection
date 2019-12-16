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


const int MAXN = 10;


int n;
int a[MAXN];


void read() {
	if (! (cin >> n))
		throw;
	forn(i,n)
		cin >> a[i];
}


int len, d[1<<MAXN];

int get_d (int msk) {
	if (msk == 0)  return true;

	int & my = d[msk];
	if (my != -1)  return my;
	my = false;

	forn(i,n)
		if (msk & (1<<i)) {
			int nmsk = msk - (1<<i);

			int end = a[i];
			for(;;) {
				if (end - a[i] + 1 >= len)
					if (get_d (nmsk))
						return my = true;
				
				++end;
				bool f = false;
				forn(j,n)
					if ((msk & (1<<j)) && a[j] == end) {
						f = true;
						nmsk -= 1<<j;
						break;
					}

				if (!f)  break;
			}
		}

	return my;
}

bool can (int len) {
	forn(i,1<<n)
		d[i] = -1;
	::len = len;

	return get_d ((1<<n)-1) != 0;
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