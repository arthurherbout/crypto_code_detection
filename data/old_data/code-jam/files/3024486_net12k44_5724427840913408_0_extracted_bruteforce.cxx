#include <bits/stdc++.h>
using namespace std;
const int limit = 100 + 5;
//i, save bullet, tower kill or not
int f[limit][limit*10][2];
int h[limit], g[limit];
int n,p,q;
int result = 0;
int d[limit];

void check(){
	int save = 0;
	int i = 1;
	while (i<=n){
		save++;
		h[i] -= q;
		if (h[i] < 0){
			if (d[i]==0) ++i;
			else while (h[i] < 0 && d[i]==1) {
				h[i] += q;
				while (h[i] >= 0){
					h[i] -= p;
					save--;
				}
				++i;
				if (save < 0) return;
				h[i] -= q;
			}
		
		}	
	}
	
	int t = 0;
	for(int i = 1; i <= n; ++i) t += g[i] * d[i];
	result = max(result , t);

}

void attempt(int i = 1){
	if (i > n) check();
	else 
	for(int j = 0; j < 2; ++j) {
		d[i] = j;
		attempt(i+1);
	}

}

void MAIN(){
	scanf("%d%d%d",&p,&q,&n);
	for(int i = 1; i <= n; ++i) scanf("%d%d",&h[i], &g[i]);
	
	result = 0;	
	attempt();
	printf("%d\n",result);
}

int main(){
	freopen("file.inp","r",stdin);
	freopen("file.out","w",stdout);
	int ntest; scanf("%d",&ntest);
	for(int test = 1; test <= ntest; ++test){
		printf("Case #%d: ", test);
		MAIN();	
	}
}