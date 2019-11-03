#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<limits>
#include<cmath>
#include<cstring>
#include<queue>
#include<algorithm>
#include<stack>
#include<map>
#include<vector>
using namespace std;

#define rep(i,n) for(int i=0; i<(n); ++i)
#define repf(i,a,b) for(int i=(a); i<=(b); ++i)
#define repd(i,a,b) for(int i=(a); i>=(b); --i)
#define ll long long
#define PB(i) push_back(i)
#define MP make_pair
#define N 160
ll a[N+10];
ll b[N+10];
ll c[N+10];
ll d[N+10];
char s[45];
int n,L;
bool dfs(int count,ll l,int k){
	if(count==0){
		rep(i,n) c[i]=a[i];
		sort(c,c+n);
		rep(i,n) if(c[i]!=b[i]) return false;
		return true;
	}
	if(L-k>count)
		if(dfs(count,l<<1,k+1))
			return true;
	if(L-k<count) return false;
	repf(i,k,L-count){
		rep(j,n)
			a[j]^=l;
		bool t=dfs(count-1,l<<1,i+1);
		rep(j,n)
			a[j]^=l;
		if(t) return t;
	}
	return false;
}
bool fun(int mid){
	rep(i,L-mid){
		if(dfs(mid,2,i+1))
			return true;
		if(mid>=1){
		rep(j,n)
			a[j]^=1;
		bool t=dfs(mid-1,2,i+1);
		rep(j,n)
			a[j]^=1;
		if(t) return t;
		}
	}
	return false;
}
int main()
{
	int test;
	freopen("in.in","r",stdin);
	freopen("out","w",stdout);
	scanf("%d",&test);
	repf(ror,1,test)
	{
		scanf("%d%d",&n,&L);
		rep(i,n){
			scanf("%s",s);
			a[i]=0;
			rep(j,L) a[i]=a[i]*2+s[j]-'0';
		}
		rep(i,n){
			scanf("%s",s);
			b[i]=0;
			rep(j,L) b[i]=b[i]*2+s[j]-'0';
		}
		sort(a,a+n);
		rep(j,n) d[j]=a[j];
		sort(b,b+n);
		int ans=L+1;
		rep(i,L+1){
           	rep(j,n) a[j]=d[j];
			if(fun(i)){
				ans=i;
				break;
			}
		}
		printf("Case #%d: ",ror);
		if(ans>L) printf("NOT POSSIBLE\n");
		else printf("%d\n",ans);
	}
	return 0;
}
