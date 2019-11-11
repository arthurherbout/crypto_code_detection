#include <iostream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cstdio>
#include <utility>
#include <algorithm>
#include <queue>
#include <stack>
#include <list>
#include <cmath>
using namespace std;

#define FOR(i,a,b) for(int i=(a);i<(b);++i)
#define FORD(i,a,b) for(int i=(b)-1;i>=(a);--i)
#define REP(i,n) FOR(i,0,n)
#define REPD(i,n) FORD(i,0,n)
#define VAR(v,w) __typeof(w) v=(w)
#define FORE(it,c) for(VAR(it,(c).begin());it!=(c).end();++it)
#define PB push_back
#define ALL(c) (c).begin(),(c).end()
#define SIZE(c) ((int)(c).size())
#define MP make_pair
#define FT first
#define SD second
typedef long long LL;
typedef unsigned long long ULL;
typedef long double LD;
typedef vector<int> VI;
typedef vector<string> VS;
typedef vector<VI> VVI;
typedef pair<int,int> PII;
typedef vector<PII> VII;
typedef vector<double> VD;
typedef vector<LD> VLD;
typedef vector<LL> VLL;
typedef vector<bool> VB;
typedef istringstream ISS;
typedef ostringstream OSS;

template<class T>
inline bool INSIDE(T x, T a, T b) { return (a <= x) && (x <= b); }

int been[100][100];
int n, m;
int res;
int ax1, ay1;
int ax2, ay2;

void go(int x, int y) {
	if (INSIDE(x,0,n-1) && INSIDE(y,0,m-1)) {
		if (!been[x][y]) {
			been[x][y] = 1;
			res++;
			go(x+ax1,y+ay1);
			go(x+ax2,y+ay2);
		}
	}
}

int main() {
	int ccc;
	cin >> ccc;
	REP(cc,ccc) {
		cin >> n >> m;
		cin >> ax1 >> ay1;
		cin >> ax2 >> ay2;
		int x, y;
		cin >> x >> y;
		REP(i,n) REP(j,m)
			been[i][j] = 0;
		res = 0;
		go(x, y);
		cout << "Case #" << cc+1 << ": " << res << endl;
		cerr << "Case #" << cc+1 << ": " << res << endl;
	}
}
