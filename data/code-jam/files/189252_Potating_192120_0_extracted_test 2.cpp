//poj 

#include<iostream>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<map>
#include<math.h>
#include<vector>
#include<set>
#include<list>
//#include<hash_set.h>
//#include<hashtable.h>

using namespace std;
typedef long long llt;

#define maxv 6

#define LEN 105
#define myAbs(x) ((x)>=0?(x):-(x))
#define Max(x,y) ((x)>(y)?(x):(y))
#define Min(x,y) ((x)<(y)?(x):(y))
#define inf 1999999999
#define MOD 200039
#define eps 1e-8
#define EPS 0.0000001
#define M_PI 3.14159265358979323846


int n,m;
int p[maxv];
int flag[LEN];
int seq[maxv];
int vis[maxv];
int res;
int ttt;


void dfs(int x){
	int i,j;
	if(x==m){
		int tmp=0;
		memset(flag, 0 , sizeof(flag));
		for(i=0;i<m;++i){
			flag[seq[i]]=1;
			for(j=seq[i]-1;j>=0;--j){
				if(flag[j]==1)
					break;
				else
					++tmp;
			}
			for(j=seq[i]+1;j<n;++j){
				if(flag[j]==1)
					break;
				else
					++tmp;
			}
		}
		if(tmp < res)
			res= tmp;
		return;
	}
	for(i=0;i<m;++i){
		if(!vis[i]){
			vis[i]=1;
			seq[x]=p[i]-1;
			dfs(x+1);
			vis[i]=0;
		}
	}
}
void solve(){
	int i,j,k;
	scanf("%d%d",&n,&m);
	for(i=0;i<m;++i){
		scanf("%d", p+i);
	}
	res= inf;
	memset(vis, 0 ,sizeof(vis));
	dfs(0);
	printf("Case #%d: %d\n",++ttt,res);
}
int main(){
//	freopen("C-small-attempt0.in","r",stdin);
//	freopen("out.txt","w",stdout);
	int t;
	scanf("%d",&t);
	ttt=0;
	while(--t>=0){
		solve();
	}
	//system("PAUSE");
	return 0;
}


