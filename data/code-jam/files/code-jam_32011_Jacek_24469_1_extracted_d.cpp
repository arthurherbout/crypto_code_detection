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

const int DX[4] = {1,0,-1,0};
const int DY[4] = {0,1,0,-1};
template<class T>
inline bool INSIDE(T x, T a, T b) { return (a <= x) && (x <= b); }

VS ma;
int df[900][30][30];
int dop[30][30];
int best[30][30];
const int inf = 1000000000;
bool been[30][30];

int main() {
	int ccc;
	cin >> ccc;
	REP(cc,ccc) {
		int n, m;
		cin >> n >> m;
		ma.clear();
		REP(i,n) {
			string s;
			cin >> s;
			ma.PB(s);
		}
		VII forr;
		REP(i,n) REP(j,m)
			if (ma[i][j] == 'T')
				forr.PB(MP(i,j));
		int f = SIZE(forr);
		REP(i,n) REP(j,m)
			dop[i][j] = inf;
		REP(ff,f) {
			REP(i,n) REP(j,m)
				df[ff][i][j] = inf;
			queue<PII> q;
			df[ff][forr[ff].FT][forr[ff].SD] = 0;
			q.push(forr[ff]);
			while (!q.empty()) {
				PII p = q.front();
				q.pop();
				int x = p.FT;
				int y = p.SD;
				REP(k,4) {
					int xx = x + DX[k];
					int yy = y + DY[k];
					if (INSIDE(xx,0,n-1) && INSIDE(yy,0,m-1) && ma[xx][yy] != '.' && df[ff][xx][yy] == inf) {
						df[ff][xx][yy] = df[ff][x][y] + 1;
						q.push(MP(xx,yy));
					}
				}
			}
			REP(i,n) REP(j,m)
				if (df[ff][i][j] < dop[i][j]) {
					dop[i][j] = df[ff][i][j];
					best[i][j] = ff;
				}
		}
		REP(i,n) REP(j,m)
			been[i][j] = 0;
		int xf = 0;
		int res = 0;
		int ff = 0;
		while (xf < f) {
			int bbest = inf;
			int a = 0, b = 0;
			if (xf) {
				REP(i,n) REP(j,m)
					if (been[i][j]) {
						REP(k,4) {
							int xx = i + DX[k];
							int yy = j + DY[k];
							if (INSIDE(xx,0,n-1) && INSIDE(yy,0,m-1) && ma[xx][yy] != '.' && !been[xx][yy]) {
								if (dop[i][j] + df[ff][xx][yy] < bbest) {
									bbest = dop[i][j] + df[ff][xx][yy];
									a = dop[i][j];
									ff = best[xx][yy];
									b = df[ff][xx][yy];
								}
							}
						}
					}
				res += (b+1)*(a+1);
			}
			queue<PII> q;
			REP(i,n) REP(j,m) {
				if (ma[i][j] != '.' && !been[i][j] && dop[i][j] == df[ff][i][j]) {
					been[i][j] = 1;
					res += df[ff][i][j];
				}
			}
			xf++;
		}
		cout << "Case #" << cc+1 << ": " << res << endl;
		cerr << "Case #" << cc+1 << ": " << res << endl;
	}
}
