#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <queue>

using namespace std;

#define FOR(i,a,b)  for(int i=(a);i<(b);++i)
#define REP(i,a)    FOR(i,0,a)
#define ZERO(m)     memset(m,0,sizeof(m))
#define ALL(x)      x.begin(),x.end()
#define PB          push_back
#define S           size()
#define LL          long long
#define LD          long double
#define MP          make_pair
#define X           first
#define Y           second
#define VC          vector
#define VI          VC<int>
#define VS          VC<string>
#define DB(a)		cout << #a << ": " << a << endl;

void print(VI v) {cerr << "[";if (v.S) cerr << v[0];FOR(i, 1, v.S) cerr << ", " << v[i];cerr << "]\n";}
void print(VS v) {cerr << "[";if (v.S) cerr << v[0];FOR(i, 1, v.S) cerr << ", " << v[i];cerr << "]\n";}
template<class T> string i2s(T x) {ostringstream o; o << x; return o.str(); }
VS splt(string s, char c = ' ') {VS rv; int p = 0, np; while (np = s.find(c, p), np >= 0) {if (np != p) rv.PB(s.substr(p, np - p)); p = np + 1;} if (p < s.S) rv.PB(s.substr(p)); return rv;}

#define PR 1000000007ll
#define PX pair < LL, int >
map < PX, LL > dp;
map < PX, LL > dp3, dp4;
LL n, b;
LL ch[100][110];
LL dp2[80][80][3000];
int go1c, go2c, go3c, go4c;

LL go2(int p, int x, int s) {
	if (s < 0) return 0;
	LL &rv = dp2[p][x][s];
	if (rv != -1) return rv;
	go2c++;
	if (x == 0) return rv = (s == 0);
	if (p == b) return rv = 0;
	rv = 0;
	rv += go2(p + 1, x, s);
	rv += go2(p + 1, x - 1, s - p);
	rv %= PR;
	//if (s <= 6 && rv) printf("go2(%d,%d,%d):%d\n", p, x, s, rv);
	return rv;
}

LL go(LL n, int x);

LL go3(LL n, int x) {
	PX px = MP(n, x);
	if (dp3.count(px)) return dp3[px];
	go3c++;
	LL rv = 0;
	LL mul = 1;
	FOR(i, 0, x + 1) {
		rv += mul * go(n, i);
		mul *= x - i;
		mul %= PR;
		rv %= PR;
	}
	return dp3[px] = rv;
}

LL go4(LL n, int x) {
	PX px = MP(n, x);
	if (dp4.count(px)) return dp4[px];
	go4c++;
	LL rv = 0;
	LL mul = 1;
	FOR(i, 1, x + 1) {
		rv += ((i * mul) % PR) * go(n, i);
		mul *= x - i;
		mul %= PR;
		rv %= PR;
	}
	return dp4[px] = rv;
}

LL go(LL n, int x) {
	if (n < 0) return 0;
	if (n == 0) return x == 0;
	PX px = MP(n, x);
	if (dp.count(px)) return dp[px];
	go1c++;
	LL rv = 0;
	int r = n % b;
	for (int ar = r; ar < 3000; ar += b) {
		LL z1 = go2(1, x, ar);
		LL z2 = go2(0, x, ar);
		rv += z1 * go3((n - ar) / b, x);
		LL df = z2 - z1;
		if (df < 0) df += PR;
		rv += df * go4((n - ar) / b, x);
	}
	rv %= PR;
	return dp[px] = rv;	
}

int main() {
	int tc;
	cin >> tc;
	FOR(atc, 1, tc + 1) {
		cin >> n >> b;
		go1c = go2c = go3c = go4c = 0;
		LL rv = 0;
		dp.clear();
		memset(dp2, -1, sizeof dp2);
		dp3.clear();
		dp4.clear();
		FOR(i, 1, b + 1) rv += go(n, i);
		rv %= PR;
		//cout << go1c << "," << go2c << "," << go3c << "," << go4c << endl;
		printf("Case #%d: %lld\n", atc, rv);
	}
}