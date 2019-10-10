#include <cstdio>
#include <iostream>
#define INFINITy 1000000000
using namespace std;
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#define MAXN 17
int n, m;
bool f[MAXN][MAXN];

int SI, SJ, FI, FJ;

struct TT{
	int i, j;
	int bi, bj, bc;
	int yi, yj, yc;
	friend bool operator <(TT a, TT b){
		if (a.i != b.i) return a.i < b.i;
		if (a.j != b.j) return a.j < b.j;
		if (a.bi != b.bi) return a.bi < b.bi;
		if (a.bj != b.bj) return a.bj < b.bj;
		if (a.bc != b.bc) return a.bc < b.bc;
		if (a.yi != b.yi) return a.yi < b.yi;
		if (a.yj != b.yj) return a.yj < b.yj;
		if (a.yc != b.yc) return a.yc < b.yc;
		return false;
	}
};

set<pair<int, TT> > q;
map<TT, int> z;

pair<int, int> wh[4][MAXN][MAXN];

void precalc(){
	for (int i = 1; i <= n; i++)
		for (int j = 1; j <= m; j++)
			if (f[i][j]){
				int p, q;

				p = i;
				q = j;
				while (f[p][q]) p--;
				wh[0][i][j] = make_pair(p, q);

				p = i;
				q = j;
				while (f[p][q]) q++;
				wh[1][i][j] = make_pair(p, q);

				p = i;
				q = j;
				while (f[p][q]) p++;
				wh[2][i][j] = make_pair(p, q);

				p = i;
				q = j;
				while (f[p][q]) q--;
				wh[3][i][j] = make_pair(p, q);

//				printf("%d %d - (%d %d) (%d %d) (%d %d) (%d %d)\n", i, j, wh[0][i][j].first, wh[0][i][j].second, 
//				wh[1][i][j].first, wh[1][i][j].second, 
//				wh[2][i][j].first, wh[2][i][j].second, 
//				wh[3][i][j].first, wh[3][i][j].second);
			}
}

int REZ;

void relax(TT const &a, int b){
	if (z.find(a)==z.end()){
		q.insert(make_pair(b, a));
		z[a] = b;
	} else{
		int tmp = z[a];
		if (tmp>b){
			q.erase(make_pair(tmp, a));
			q.insert(make_pair(b, a));
			z[a] = b;
		}
	}
}

int solve(){
	precalc();
	q.clear();
	z.clear();
	TT be, t;
	be.i = SI;
	be.j = SJ;

	be.bi = wh[0][SI][SJ].first;
	be.bj = wh[0][SI][SJ].second;
	be.bc = 0;

	be.yi = wh[0][SI][SJ].first;
	be.yj = wh[0][SI][SJ].second;
	be.yc = 0;

	relax(be, 0);

	REZ = INFINITy;

	int co = 0;

	while (!q.empty()){
		co++;
		if (co%10000==0) cerr<<q.size()<<" "<<z.size()<<endl;
		be = (*(q.begin())).second;
		int dst = (*(q.begin())).first;
//		printf("%d %d   %d %d %d   %d %d %d - %d\n", be.i, be.j, be.bi, be.bj, be.bc, be.yi, be.yj, be.yc, dst);
		if ((be.i==FI)&&(be.j==FJ)) return dst;
		q.erase(make_pair(dst, be));

		for (int i = 0; i < 4; i++){
			t = be;
			t.bi = wh[i][t.i][t.j].first;
			t.bj = wh[i][t.i][t.j].second;
			t.bc = i;
			relax(t, dst);
		}

		for (int i = 0; i < 4; i++){
			t = be;
			t.yi = wh[i][t.i][t.j].first;
			t.yj = wh[i][t.i][t.j].second;
			t.yc = i;
			relax(t, dst);
		}

		for (int i = 0; i < 4; i++){
			t = be;
			if (i==0) t.i--;
			if (i==1) t.j++;
			if (i==2) t.i++;
			if (i==3) t.j--;
			if (f[t.i][t.j]){
				relax(t, dst+1);
			}
			else{
				if ((t.i==t.bi)&&(t.j==t.bj)){
					t.i = t.yi;
					t.j = t.yj;
					if (t.yc==0) t.i++;
					if (t.yc==1) t.j--;
					if (t.yc==2) t.i--;
					if (t.yc==3) t.j++;
					relax(t,dst+1);
				}
				if ((t.i==t.yi)&&(t.j==t.yj)){
					t.i = t.bi;
					t.j = t.bj;
					if (t.bc==0) t.i++;
					if (t.bc==1) t.j--;
					if (t.bc==2) t.i--;
					if (t.bc==3) t.j++;
					relax(t, dst+1);
				}
			}
		}




	}
//	printf("zogs: %d %d\n", q.size(), z.size());

	return REZ;
}

int main(){
	freopen("cake.in", "rt", stdin);
	freopen("cake.out", "wt", stdout);
	int T;
	scanf("%d\n", &T);
	for (int t = 1; t <= T; t++){
		scanf("%d%d\n", &n, &m);
		for (int i = 0; i < MAXN; i++) for (int j = 0; j < MAXN; j++) f[i][j] = false;
		for (int i = 0; i < n; i++){
			for (int j = 0; j < m; j++){
				char ch;
				scanf("%c", &ch);
				if (ch=='O'){
					SI = i+1;
					SJ = j+1;
					ch = '.';
				}
				if (ch=='X'){
					FI = i+1;
					FJ = j+1;
					ch = '.';
				}
				f[i+1][j+1] = (ch=='.');
			}
			scanf("\n");
		}
		printf("Case #%d: ", t);
		int tmp = solve();
		if (tmp==INFINITy) printf("THE CAKE IS A LIE\n"); else printf("%d\n", tmp);
	}
	fclose(stdin);
	fclose(stdout);
	return 0;
}
