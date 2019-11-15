#include <iostream>
#include <fstream>
#include <cstdio>
#include <cmath>
#include <vector>
#include <cstring>
#include <string>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <algorithm>
using namespace std;

#define REP(i,n) for(int i=0; i<n; ++i)
#define FOR(i,a,b) for(int i=a; i<=b; ++i)
#define FORR(i,a,b) for (int i=a; i>=b; --i)
#define ALL(c) (c).begin(), (c).end()

typedef long long ll;
typedef vector<int> VI;
typedef vector<ll> VL;
typedef vector<VI> VVI;
typedef pair<int,int> P;
typedef pair<ll,ll> PL;

int dp[101][101][101][101];
const int INF = 1e9;

int solve(int hd, int ad, int hk, int ak, int b, int d){
	REP(i,hd+1) REP(j,hk+1) REP(k,hk+1) REP(l,ak+1) dp[i][j][k][l] = INF;
	dp[hd][ad][hk][ak] = 0;
	queue<pair<P, P> > que;
	que.push(make_pair(P(hd, ad), P(hk, ak)));
	while (!que.empty()){
		pair<P,P> pp = que.front();
		que.pop();
		int hd_p, ad_p, hk_p, ak_p;
		hd_p = pp.first.first;
		ad_p = pp.first.second;
		hk_p = pp.second.first;
		ak_p = pp.second.second;
		// printf("%d %d %d %d\n", hd_p, ad_p, hk_p, ak_p);
		if (hd_p == 0 || hk_p == 0) continue;
		REP(k,4){
			int hd_n = hd_p, ad_n = ad_p, hk_n = hk_p, ak_n = ak_p;
			switch(k){
				case 0 :
					hk_n = max(0, hk_p - ad_p);
					hd_n = hd_p - ak_p;
					break;
				case 1 :
					ad_n = min(hk, ad_p + b);
					hd_n = hd_p - ak_p;
					break;
				case 2 :
					hd_n = hd - ak_p;
					break;
				case 3 :
					ak_n = max(0, ak_p - d);
					hd_n = hd_p - ak_n;
					break;
			}

			hd_n = max(0, hd_n);

			if (dp[hd_n][ad_n][hk_n][ak_n] != INF) continue;
			// printf("	%d %d %d %d\n", hd_n, ad_n, hk_n, ak_n);
			dp[hd_n][ad_n][hk_n][ak_n] = dp[hd_p][ad_p][hk_p][ak_p] + 1;
			que.push(make_pair(P(hd_n, ad_n), P(hk_n, ak_n)));
		}
	}
	int ans = INF;
	REP(i,hd+1) REP(j,hk+1) REP(k,ak+1) ans = min(ans, dp[i][j][0][k]);
	return (ans == INF ? -1 : ans);
}

int main(void) {
	ifstream ifs("input.txt");
	ofstream ofs("out.txt");
	FILE *fp;
	fp = fopen("out.txt","w");
	int num_of_cases;
	ifs >> num_of_cases;
	REP(cas,num_of_cases){
		fprintf(fp,"Case #%d: ",cas+1);
		printf("Case #%d: ",cas+1);

		int hd, ad, hk, ak, b, d;
		ifs >> hd >> ad >> hk >> ak >> b >> d;

		int ans = solve(hd, ad, hk, ak, b, d);
		cout << ans << endl;
		if (ans == -1) fprintf(fp, "IMPOSSIBLE\n");
		else fprintf(fp, "%d\n", ans);
	}

	return 0;
}