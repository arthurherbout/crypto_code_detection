#include <iostream>
#include <vector>
#include <string>
#include <memory.h>
#include <algorithm>
#include <set>
#include <queue>
#include <sstream>
#include <list>
#include <map>
#include <cmath>

#include <memory.h>

using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define FORE(i,a,b) for(int i = (a); i <= (b); ++i)
#define FORD(i,a,b) for(int i = (a); i >= (b); --i)
#define FIR(k) FOR(i,0,k)
#define FJR(k) FOR(j,0,k)
#define FI FIR(n)
#define FJ FJR(n)
#define ALL(v) v.begin(), v.end()
#define LL long long

typedef vector<int> VI;
typedef pair<LL, int> PLI;
typedef vector<PLI> VPLI;

int S[20][20], W[20][20], T[20][20];
int n, m;

#define INF 1000000000000000LL
LL best[50][50];

int enc(int r, int c) {return (r << 8) | c; }
void dec(int x, int& r, int& c) {r=x>>8; c = x & 255;}

void crossing(int r, int c, int& s, int& w, int &t) {
	int y = r/2, x = c/2;
	s = S[y][x];
	w = W[y][x];
	t = T[y][x] % (w+s);
}

int main() {
freopen("B-large.in", "rt", stdin);
freopen("B-large.out", "w", stdout);	

	int cases; cin >> cases;

	FOR(tc, 0, cases) {
		cin >> n >> m;
		FIR(50)FJR(50) best[i][j]=INF;
		FI FJR(m) cin >> S[i][j] >> W[i][j] >> T[i][j];
		int dest_col = 2*m-1;
		priority_queue< PLI, VPLI, greater<PLI> > q;
		q.push(make_pair(0LL, enc(2*n-1, 0)));
		best[2*n-1][0] = 0;

		while (!q.empty()) {
			PLI p = q.top(); q.pop();
			int r, c; dec(p.second, r, c);
			LL time = p.first;

			int nr = r%2 ? r+1 : r-1;
			if (nr >= 0 && nr < 2*n) {
				LL ntime = time + 2;
				if (ntime < best[nr][c]) {
					best[nr][c] = ntime;
					q.push(make_pair(ntime, enc(nr, c)));
				}
			}

			int nc = c%2 ? c+1 : c-1;
			if (nc >= 0 && nc <= dest_col) {
				LL ntime = time + 2;
				if (ntime < best[r][nc]) {
					best[r][nc] = ntime;
					q.push(make_pair(ntime, enc(r, nc)));
				}
			}

			int s, w,t;
			
			crossing(r, c, s, w, t);
			LL tt = time % (s+w);
			if (tt < t) tt += s+w;

			int wait = 0;
			if (t + s <= tt)  wait = t+s+w - tt;

			nr = r%2 ? r-1 : r+1;
			LL ntime = time+wait+1;
			if (ntime < best[nr][c]) {
				best[nr][c] = ntime;
				q.push(make_pair(ntime, enc(nr, c)));
			}

			wait = 0;
			if (t + s > tt)  wait = t+s-tt;

			nc = c%2 ? c-1 : c+1;
			ntime = time+wait+1;
			if (ntime < best[r][nc]) {
				best[r][nc] = ntime;
				q.push(make_pair(ntime, enc(r, nc)));
			}
		}

		printf("Case #%d: %lld\n", tc+1, best[0][dest_col]);
	}
}
