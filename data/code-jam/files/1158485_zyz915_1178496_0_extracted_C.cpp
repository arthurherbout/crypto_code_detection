#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <queue>
#include <ctime>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <cmath>
#include <string>

using namespace std;

#define MP make_pair
#define PB push_back

char g[10][10];
int T,ti=0,n,m;
int in[10][10];

bool tryit(int st){
	memset(in,0,sizeof(in));
	for (int i=0;i<n;i++)
	for (int j=0;j<m;j++){
		if (g[i][j]=='-'){
			if (st&(1<<(i*m+j))) in[i][(j+m-1)%m]++;
			else in[i][(j+1)%m]++;
		}
		if (g[i][j]=='|'){
			if (st&(1<<(i*m+j))) in[(i+n-1)%n][j]++;
			else in[(i+1)%n][j]++;
		}
		if (g[i][j]=='\\'){
			if (st&(1<<(i*m+j))) in[(i+n-1)%n][(j+m-1)%m]++;
			else in[(i+1)%n][(j+1)%m]++;
		}
		if (g[i][j]=='/'){
			if (st&(1<<(i*m+j))) in[(i+1)%n][(j+m-1)%m]++;
			else in[(i+n-1)%n][(j+1)%m]++;
		}
	}
	for (int i=0;i<n;i++)
	for (int j=0;j<m;j++) if (in[i][j]>1) return false;
	return true;
}

int main(){
	for (scanf("%d",&T);T--;){
		scanf("%d%d",&n,&m);
		for (int i=0;i<n;i++) scanf("%s",g[i]);
		int ans=0;
		for (int st=0;st<(1<<(n*m));st++) ans+=tryit(st);
		printf("Case #%d: %d\n",++ti,ans%1000003);
	}
    return 0;
}
