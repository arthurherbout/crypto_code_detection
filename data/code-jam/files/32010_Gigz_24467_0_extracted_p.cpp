#include <cstdio>
#include <algorithm>
#include <vector>
#include <cmath>
#include <map>
#include <string>

using namespace std;

#define FOR(i,n) for(i = 0; i < (n); ++i)
#define SFOR(i,m,n) for(i = (m); i < (n); ++i)
#define pb push_back

int L[400], R[400], C[400];
map<string, int> M;

int Q[10000];

int main()
{
	FILE *fi = fopen("p.in", "rt");
	FILE *fo = fopen("p.out", "wt");
	int t,tt;
	fscanf(fi,"%d",&tt);
	FOR(t,tt)
	{
		M.clear();
		int res = 10000;
		int i,j,cnt = 0;
		int n;
		fscanf(fi,"%d",&n);
		FOR(i,n){
			char buf[1000];
			int l, r;
			fscanf(fi, "%s%d%d", buf, &l, &r);
			string s(buf);
			if (M[s] == 0) M[s] = ++cnt;
			L[i] = l - 1;
			R[i] = r - 1;
			C[i] = M[s] - 1;
		}
		FOR(i,1<<n){
			int U[300];
			memset(U,0,sizeof(U));
			int cr = 0;
			FOR(j,n) if (i & (1<<j)) {U[C[j]] = 1; ++cr;}
			int cur = 0;
			FOR(j,n) cur += U[j];
			if (cur > 3) continue;
			memset(Q,0,sizeof(Q));
			FOR(j,n) if (i & (1<<j)) for(int k = L[j]; k <= R[j]; ++k) Q[k] = 1;
			int fl = 1;
			FOR(j,10000) if (!Q[j]) {fl = 0; break;}
			if (fl && cr < res) res = cr;
		}
		if (res < 10000)
			fprintf(fo,"Case #%d: %d\n",t+1, res);
		else
			fprintf(fo,"Case #%d: IMPOSSIBLE\n",t+1);
	}
	fclose(fi);
	fclose(fo);
	return 0;
}