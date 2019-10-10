#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <fstream>
#include <map>
#include <set>
#include <queue>
#include <memory.h>

using namespace std;

typedef vector<int> VI;
typedef pair<int, int> PI;
typedef vector<PI> VPI;

#define FOR(i,a,n) for (int i = (a); i < (n); ++i)
#define FORE(i,a,n) for (int i = (a); i <= (n); ++i)
#define FORD(i,a,b) for (int i = (a); i >= (b); --i)
#define REP(i,n) FOR(i,0,n)
#define REPE(i,n) FORE(i,0,n)
#define LL long long
#define FIR(n) REP(i,n)
#define FJR(n) REP(j,n)
#define ALL(v) v.begin(), v.end()

#define FI FIR(n)
#define FJ FJR(n)
#define FR(i,a) FOR(i,a,n)
#define REPN(i) REP(i,n)

#define GI(n) scanf("%d", &n)
#define GI2(n,m) scanf("%d %d", &n, &m)

int g[50][50];
LL mask[50];

set<LL> f[50];

#define INF 1000000

int n;

int bc(LL n) {
	int res =0;
	while (n) n &= n-1, ++res;
	return res;
}

int main() {
freopen("D-small-attempt0.in", "rt", stdin);
freopen("D-small-attempt0.out", "w", stdout);

	int ntc; GI(ntc);
	
	FORE(tc, 1, ntc) {
		int e;
		GI2(n, e);

		FI FJ g[i][j] = i == j ? 0 : INF;

		FIR(e) {
			int a, b;
			scanf("%d,%d", &a, &b);
			g[a][b]= g[b][a] =1;
		}
		FI mask[i] = 1LL << i;
		FI FJ if (g[i][j] < INF) mask[i] |= 1LL << j;


		REP(k, n) FI FJ g[i][j] = min(g[i][j], g[i][k] + g[k][j]);
		f[0].insert(mask[0]);

		int res = g[0][1];
		VI cur; cur.push_back(0);

		FORD(dist, res, 2) {
			VI next;

			int q = dist-1;
			FI if (g[1][i] == q && g[0][i] == res-q) next.push_back(i);

			FIR(next.size())
				FJR(cur.size())
				if(g[cur[j]][next[i]] == 1) {
					set<LL>& dest = f[next[i]];
					set<LL>& source= f[cur[j]];
					LL mm = mask[next[i]];

					for (set<LL>::const_iterator it = source.begin(); it != source.end(); ++it)
						dest.insert((*it) | mm);
				}

			cur.clear();
			swap(next, cur);
		}

		int nb = 0;
		FIR(cur.size()) {
			set<LL>& source= f[cur[i]];
			for (set<LL>::const_iterator it = source.begin(); it != source.end(); ++it)
				nb = max(nb, bc(*it));
		}



		printf("Case #%d: %d %d\n", tc, res-1, nb-res);
		FI f[i].clear();
	}
}
