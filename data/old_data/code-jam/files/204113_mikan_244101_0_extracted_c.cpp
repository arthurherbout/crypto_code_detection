#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cstdlib>
#include<cmath>
#include<string>
#include<vector>
#include<map>
#include<set>
#include<queue>
#include<bitset>
#include<cassert>

using namespace std;

typedef long long ll;
typedef pair<int,int> pint;
typedef vector<int> vint;

#define mp make_pair
#define pb push_back
#define REP(i,a,b) for(int i=a;i<b;++i)
#define rep(i,n) REP(i,0,n)

int t,k,n;
int p[102][27];
vint ts;
int pt;
void visit(int x,vector<vint> &g,vint &goed){
//	cout<<"vis "<<x<<endl;
	goed[x]=1;
	rep(i,n)if(g[x][i])if(!goed[i])visit(i,g,goed);
	ts[pt++]=x;
}

int main(){
	freopen("c.in","r",stdin);
	freopen("c.out","w",stdout);
	cin>>t;
	REP(cas,1,t+1){
		cin>>n>>k;
		rep(i,n)rep(j,k)scanf("%d",&p[i][j]);
		vector<vint> g(n,vint(n,0));
		rep(i,n)rep(j,n){
			rep(hoge,k)if(p[i][hoge]>=p[j][hoge])goto tugi;
			g[i][j]=1;
			tugi:;
		}
//		rep(i,n){
//			rep(j,n)printf("%d ",g[i][j]);
//			cout<<endl;
//		}
		vint goed(n,0);
		pt=0;
		ts.resize(n);
		rep(x,n)if(!goed[x])visit(x,g,goed);
//		rep(x,n)printf("%d ",ts[x]);cout<<endl;
		vector<int> h;
		rep(i,n){
			rep(j,h.size()){
				if(g[i][h[j]] || g[h[j]][i]){
					h[j]=i;
					goto ggg;
				}
			}
			h.pb(i);
			ggg:;
		}
		printf("Case #%d: %d\n",cas,h.size());
	}
	return 0;
}
