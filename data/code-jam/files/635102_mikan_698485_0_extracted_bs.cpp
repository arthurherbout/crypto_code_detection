#include<cstdio>
#include<iostream>
#include<sstream>
#include<algorithm>
#include<vector>
#include<string>
#include<queue>
#include<set>
#include<map>
#include<cstring>
#include<cstdlib>
//#include<cmath>
//#include<gmpxx.h>

using namespace std;

typedef pair<int,int> pint;
typedef vector<int> vint;
typedef long long ll;
//typedef mpz_class Bigint;

#define pb push_back
#define mp make_pair
#define repp(i,a,b) for(int i=(int)(a);i<=(int)(b);++i)
#define rep(i,n) repp(i,0,(n)-1)

int t,p;

int in(){
	int n;
	scanf("%d",&n);
	return n;
}
int m[1<<11];
int pr[12][1<<10];
int main(){
	t=in();
	repp(cas,1,t){
		p=in();
//		cout<<(1<<p)<<endl;
		for(int i=0;i<(1<<p);++i)m[i]=p-in();
		rep(i,p)for(int j=0;j<(1<<(p-i-1));++j)pr[i][j]=in();
//		for(int i=0;i<(1<<p);++i)cout<<m[i]<<" ";cout<<endl;
		vint ch(1<<p,0);
		rep(i,1<<p){
			int now=1;
			rep(j,m[i]){
				ch[now]=1;
				if(i&(1<<(p-j-1)))now=2*now+1;
				else now=2*now;
			}
		}
		int ans=0;
		rep(i,1<<p)ans+=ch[i];
		printf("Case #%d: %d\n",cas,ans);

	}
	return 0;
}
