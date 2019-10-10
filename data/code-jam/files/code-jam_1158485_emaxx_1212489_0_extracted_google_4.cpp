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


string s;


void read() {
	if (! (cin >> s))
		throw;
}


int64 my_sqrt (int64 n) {
	int64 l = 0,  r = min (n, (1ll<<30) + 1000);
	while (l < r - 1) {
		int64 m = (l + r) >> 1;
		if (m * m < n)
			l = m;
		else
			r = m;
	}

	for (int64 ans=l; ans<=r; ++ans)
		if (ans * ans == n)
			return ans;
	return -1;
}

int solve (const string & s) {
	int64 n = 0;
	forn(i,s.length())
		if (s[i] == '1')
			n |= 1ll << (s.size()-1 - i);

	return (int) my_sqrt (n);
}


void solve() {
	vector<int> v;
	forn(i,s.length())
		if (s[i] == '?')
			v.pb (i);
	int n = (int)v.size();

	forn(msk,1<<n) {
		forn(i,v.size())
			s[v[i]] = (msk & (1<<i)) ? '1' : '0';
		int ans = solve (s);
		if (ans != -1) {
			puts (s.c_str());
			return;
		}
	}
	throw;
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