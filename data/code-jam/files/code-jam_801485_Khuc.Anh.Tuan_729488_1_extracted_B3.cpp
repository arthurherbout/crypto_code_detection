#include <iostream>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;

#define Rep(i,n) for(int i=0;i<n;++i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)
#define fi first
#define se second
#define MP make_pair
#define pb push_back

typedef vector<int> VI;
typedef pair<int,int> PII;

int n;
VI ke[1010];
int st, res;
bool vs[1010];
int maxlvl;
int level[1010];
int a[1010][1010];

int ds[100010];
int nd;

int trc[1010];


void go(int i, int step) {
	if(step > 10 * n) return;
	vs[i] = true;
	ds[nd++] = i;
	
	if(a[i][st]) {
		// process here
		int total = step;
		memset( trc, -1, sizeof(trc));
		
		Rep(k,nd) {
			int id = ds[k];
			if(trc[id] == -1) trc[id] = k;
			else {
				total -= k - trc[id];
			}
		}
		
		res = max( res, total);
	}
	
	int jmax = -1;
	int lvlmax = -1;
	
	Rep(kk,ke[i].size()) {	
		int j = ke[i][kk]; 
		if(!vs[j]) {
			if(level[j] > lvlmax && level[j] <= maxlvl) {
				lvlmax = level[j];
				jmax = j;
			}
		}
	}
	
	if(jmax!=-1) {
		go( jmax, step + 1);
	}
	else {
		Rep(kk, ke[i].size()) {
			int j = ke[i][kk];
			if(level[j] <= maxlvl && level[j] > lvlmax) {
				lvlmax = level[j];
				jmax = j;
			}
		}
		
		go( jmax, step + 1);
	}
	
	vs[i] = false;
}

int main() {
	int ntest;
	scanf("%d", &ntest);

	For(test,1,ntest) {
		// int n;		
		cerr << test << endl;
		scanf("%d", &n);
		
		For(i,1,n) ke[i].resize(0);
		
		memset(level,0,sizeof(level));
		memset( a, 0, sizeof(a));
		
		For(i,1,3) For(j,1,3) if(i!=j) ke[i].pb(j), a[i][j] = true;
		
		level[2] = 1;
		level[3] = 2;
		
		For(i,4,n) {
			int u,v;
			scanf("%d%d",&u,&v);
			a[i][u] = a[u][i] = a[i][v] = a[v][i] = true;
			ke[i].pb(u); ke[i].pb(v);
			ke[v].pb(i); ke[u].pb(i);
			
			level[i] = max(level[u],level[v])+1;
		}
		
		res = 0;
		For(i,1,n) {
			st = i;
			maxlvl = 1000000;
			memset(vs,0,sizeof(vs));
			nd = 0;
			go(st, 1);
		}
		
		printf("Case #%d: ", test);
		printf("%d\n", res);
		fflush(stdout);
	}
    return 0;
}
