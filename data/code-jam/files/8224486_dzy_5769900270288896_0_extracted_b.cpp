#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;
typedef long long ll;
const int mo=1e9+7;
int pow(int a,int b,int c){int ret=1;for(;b;b>>=1,a=1LL*a*a%c)if(b&1)ret=1LL*ret*a%c;return ret;}
int n,m,K,a[17][17];
int main(){
	freopen("data2.in", "r", stdin);
	freopen("data2.out", "w", stdout);
	int tc, T=0;
	scanf("%d",&tc);
	while(tc--){
		scanf("%d%d%d",&n,&m,&K);
		int ans=1e9;
		for(int i=0;i<(1<<(n*m));i++){
			int cnt=0,tot=0;
			for(int j=1;j<=n;j++) for(int k=1;k<=m;k++){
				if(i>>tot&1) a[j][k]=1,cnt++; else a[j][k]=0;  
				tot++;
			}
			if(cnt==K){
				int ret=0;
				for(int j=1;j<=n;j++) for(int k=1;k<m;k++) if(a[j][k]&&a[j][k+1])ret++;
				for(int j=1;j<n;j++) for(int k=1;k<=m;k++) if(a[j][k]&&a[j+1][k])ret++;
				ans=min(ans,ret);
			}
		}
		printf("Case #%d: %d\n",++T,ans); 
	}
	return 0;
}

