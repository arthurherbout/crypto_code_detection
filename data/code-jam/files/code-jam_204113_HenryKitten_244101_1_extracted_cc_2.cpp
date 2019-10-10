#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <unistd.h>

#define ll long long

using namespace std;


int main(){
	int ncas;
	scanf("%d",&ncas);
	for(int cas=1;cas<=ncas;cas++){
		int n,k;
		bool conn[128][128] = {0};
		int g[128][32];
		scanf("%d %d",&n,&k);

		for(int i=0;i<n;i++){
			for(int j=0;j<k;j++){
				cin >> g[i][j];
			}
		}

		int deg[128] ={0};
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
//				printf("%d %d == \n",i,j);
				if(g[i][0] == g[j][0]){
					conn[j][i] = conn[i][j] = 1;
				}else{
					for(int l=1;l<k;l++){
						if(g[i][l] == g[j][l]){
							conn[j][i] = conn[i][j] = 1;
							break;
						}
						int dij = (g[i][l-1] - g[j][l-1]) > 0 ? 1 : -1,cij = (g[i][l] - g[j][l]) > 0 ? 1 : -1;
						if(dij * cij < 0){
							conn[j][i] = conn[i][j] = 1;
							break;
						}

					}
				}
				deg[i] += conn[i][j];
			}
		}
		/*
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				printf("%d ",conn[i][j]);
			}
			putchar(10);
		}*/

		int nset = 0;
		int done = 0;
		bool used[128] = {0};
		while(done < n){
			bool curused[128] = {0};
			bool flag = 1;
			while(flag){
				int maxi = 0;
				int maxidx;
				flag = 0;
				for(int i=0;i<n;i++){
					if(deg[i] && !used[i] && !curused[i]){
						if(deg[i] > maxi){
							maxi = deg[i];
							maxidx = i;
							flag = 1;
						}
					}
				}
				if(!flag) break;
				used[maxidx] = 1;
				done++;
				for(int i=0;i<n;i++){
					if(conn[maxidx][i]){
						curused[i] = 1;
						deg[i]--;
					}
				}
			}
			nset++;
			if(nset > n) break;
		}
		printf("Case #%d: %d\n",cas,nset);
	}
	return 0;
}
