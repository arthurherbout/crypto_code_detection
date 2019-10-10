#include <unordered_set>
#include <unordered_map>
#include <functional>
#include <algorithm>
//#include <iostream>
#include <numeric>
#include <cassert>
#include <cstring>
#include <cstdio>
#include <vector>
#include <queue>
//#include <cmath>
#include <set>
#include <map>
using namespace std;

typedef long long LL;
typedef unsigned long long ULL;
typedef vector<int> VI;
typedef vector<LL> VLL;
typedef vector<VI> VVI;
typedef pair<int,int> PII;
typedef vector<PII> VPII;

#define REP(i,n) for(int i=0;i<(n);++i)
#define FOR(i,b,e) for(int i=(b);i<=(e);++i)
#define FORD(i,b,e) for(int i=(b);i>=(e);--i)

#define PB push_back
#define ALL(V) (V).begin(),(V).end()
#define SIZE(V) ((int)(V).size())

#define MP make_pair
#define ST first
#define ND second

#define DBG

#ifdef DBG
	#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
	#define debug(...)
#endif

int __stmp;
#define scanf __stmp=scanf


const int INF = 1000000001;
const int MAX = 100000;

#define PARALLEL 1

/* UWAGA NA PAMIEC!
   Moze byc potrzebne nawet Z razy wiecej pamieci w trakcie dzialania,
   chyba ze bedzie sie ja alokowac i zwalniac w solve() wtedy moze byc
   potrzebne do <liczba rdzeni> razy wiecej.
   Kompilowac z --openmp
 */

class solver {
	public:
		// wczytujemy caly input dla jednego zestawu danych
		void input() {
			scanf("%d", &n);
			C.resize(n);
			REP(i,n) scanf("%d", &C[i]);
			G.assign(n, VI());
			REP(i,n-1)
			{
				int a;
				scanf("%d", &a);
				a--;
				G[a].PB(i);
				G[i].PB(a);
			}
		}
		
		int sum_dfs(int u, int p) {
			auto it = sub_sum.find({u, p});
			if(it != sub_sum.end()) return it->ND;
			int sum = 0;
			for(int v : G[u])
				if(v != p)
					sum = max(sum, sum_dfs(v, u));
			sum += C[u];
			sub_sum[PII(u, p)] = sum;
			return sum;
		}
		
		int dfs_solve(int u, int p, int f1, int f2, int f3) {
			if(u == f1 || u == f2 || u == f3) return 0;
			int ret = 0;
			for(int v : G[u])
				if(v != p)
					ret = max(ret, dfs_solve(v, u, f1, f2, f3));
			return ret + C[u];
		}
		
		int solve(int a, int ap, int b, int bp) {
			auto it = dp.find({{a, ap}, {b, bp}});
			if(it != dp.end()) return it->ND;
			if(a == b) {
				int res = -INF;
				for(int v : G[a])
					if(v != ap && v != bp) {
						int my_sum = C[a] + sub_sum[PII(v, a)];
						int ot_sum = 0;
						for(int vv : G[a])
							if(vv != ap && vv != bp && vv != v)
								ot_sum = max(ot_sum, sub_sum[PII(vv, a)]);
						res = max(res, my_sum - ot_sum);
					}
				if(res == -INF) res = C[a];
				dp[MP(PII(a, ap), PII(b, bp))] = res;
				return res;
			}
			int res = -INF;
			for(int v : G[a])
				if(v != ap && v != nxt[a][b]) {
					int my_sum = C[a] + sub_sum[PII(v, a)];
					int ctmp = C[a];
					C[a] = 0;
					int ot_sum = dfs_solve(b, -1, ap, bp, v);
					C[a] = ctmp;
					res = max(res, my_sum - ot_sum);
				}
			res = max(res, C[a] - solve(b, bp, nxt[a][b], a));
			dp[MP(PII(a, ap), PII(b, bp))] = res;
			return res;
		}
		
		void dfs_nxt(int u, int p, int a, int k) {
			nxt[a][u] = k;
			for(int v : G[u])
				if(v != p)
					dfs_nxt(v, u, a, k);
		}
		
		// skminiamy rozwiazanie
		void solve() {
			REP(p,n)
				for(int u : G[p])
					sum_dfs(u, p);
			nxt.assign(n, VI(n, -1));
			REP(u,n)
				for(int v : G[u])
					dfs_nxt(v, u, u, v);
			tres = -INF;
			REP(i,n)
			{
				int best = INF;
				REP(j,n)
				{
					best = min(best, solve(i, -1, j, -1));
				}
				tres = max(tres, best);
			}
			sub_sum.clear();
			dp.clear();
		}
		
		// wypisujemy output
		void output() {
			printf("%d\n", tres);
		}
	private:
		int n;
		VI C;
		VVI G, nxt;
		int tres;
		map<PII,int> sub_sum;
		map<pair<PII,PII>,int> dp;
};

int main(int argc, char *argv[]) {
	int case_id = argc == 2 ? atoi(argv[1])-1 : -1;
	int Z;
	scanf("%d", &Z);
	vector<solver> S(Z);
	REP(z,Z) S[z].input();
	if(case_id == -1) {
		#if PARALLEL == 1
			#pragma omp parallel for schedule(dynamic)
		#endif
		REP(z,Z) S[z].solve();
	} else {
		S[case_id].solve();
	}
	REP(z,Z)
	{
		if(case_id == -1 || z == case_id) {
			printf("Case #%d: ", z+1);
			S[z].output();
		}
	}
	return 0;
}
