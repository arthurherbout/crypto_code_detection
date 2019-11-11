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


const string CH = "oieastbg";


int k;
string s;


void read() {
	cin >> k >> s;
}


bool g[100][100];
int din[100], dout[100];


void solve() {
	memset (g, 0, sizeof g);
	forn(i,s.length()-1) {
		int a = s[i] - 'a',
			b = s[i+1] - 'a',
			na = CH.find(s[i])==-1 ? a : 26+(int)CH.find(s[i]),
			nb = CH.find(s[i+1])==-1 ? b : 26+(int)CH.find(s[i+1]);
		g[a][b] = g[na][b] = g[a][nb] = g[na][nb] = true;
	}

	int ans = 0;
	memset (din, 0, sizeof din);
	memset (dout, 0, sizeof dout);
	forn(i,40)
		forn(j,40)
			if (g[i][j]) {
				++dout[i];
				++din[j];
				++ans;
			}

	int add = 0;
	for(;;) {
		int v1 = -1,  v2 = -1;
		forn(i,40)
			if (din[i] < dout[i])
				v1 = i;
			else if (din[i] > dout[i])
				v2 = i;
		if (v1 == -1 && v2 == -1)  break;
		if (v1 != -1 && v2 != -1) {
			++din[v1];
			++dout[v2];
			++add;
			continue;
		}
		throw;
	}

	ans += max (0, add - 1);
	
	cout << ans+1 << endl;
}


int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	forn(tt,ts) {
		read();
		if (! cin)  throw;
		printf ("Case #%d: ", tt+1);
		solve();
	}
}