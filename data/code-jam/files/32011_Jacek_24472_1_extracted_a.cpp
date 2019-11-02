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

struct R {
	int a, b, c;
};

struct B {
	int p;
	bool cl;
};

bool byA(const R& r1, const R& r2) {
	if (r1.a < r2.a)
		return true;
	if (r1.a > r2.a)
		return false;
	if (r1.b < r2.b)
		return true;
	if (r1.b > r2.b)
		return false;
	if (r1.c < r2.c)
		return true;
	if (r1.c > r2.c)
		return false;
	return false;
}

bool operator<(const B& b1, const B& b2) {
	if (b1.p < b2.p)
		return true;
	if (b1.p > b2.p)
		return false;
	if (b1.cl < b2.cl)
		return true;
	if (b1.cl > b2.cl)
		return false;
	return false;
}

int main() {
	int ccc;
	cin >> ccc;
	REP(cc,ccc) {
		int res = 0;
		vector<R> req;
		int n;
		cin >> n;
		REP(i,n) {
			R r;
			cin >> r.a >> r.b >> r.c;
			req.PB(r);
		}
		sort(ALL(req),byA);
		REP(i,n) {
			int a = req[i].a;
			vector<B> br;
			REP(j,i+1) {
				int b1 = req[j].b;
				int b2 = 10000-a-req[j].c;
				if (b1 <= b2) {
					B bb;
					bb.p = b1;
					bb.cl = 0;
					br.PB(bb);
					bb.p = b2;
					bb.cl = 1;
					br.PB(bb);
				}
			}
			sort(ALL(br));
			int x = 0;
			FORE(it,br) {
				if (it->cl)
					x--;
				else
					x++;
				res = max(res, x);
			}
		}
		cout << "Case #" << cc+1 << ": " << res << endl;
		cerr << "Case #" << cc+1 << ": " << res << endl;
	}
}
