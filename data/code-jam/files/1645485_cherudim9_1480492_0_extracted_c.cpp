#include <vector>
#include <cstdio>
#include <set>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <numeric>
#include <queue>
#include <iostream>
#include <string>
#include <cstring>
#include <utility>
#define sz(a) int((a).size())
#define pb push_back
#define mk make_pair
#define fi first
#define se second
#define Rep(i,j,k) for (int i=(j); i<=(k); i++)
#define Repd(i,j,k) for (int i=(j); i>=(k); i--)
#define ALL(c) (c).begin(),(c).end()
#define TR(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define SUM(a) accumulate(all(a),string())
#define online1
using namespace std;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef pair<int,int> II;
typedef long long LL;

const double eps=1e-12;
int n,s[10],tot;
II p[10];

struct T{
	int x, y;
	double t;
	bool operator<(T b)const{ return t-b.t<-eps; }
} a[1000];

double ans=0.0;

bool examine(double t0, double t1){
	t1-=t0;
	Rep(x,1,n)
	Rep(y,1,n) if (p[x].fi==p[y].fi && x!=y){
		double s0=s[x], s1=s[y],
				p0=p[x].se+t0*s0,
				p1=p[y].se+t0*s1;
		if (s0<s1) swap(s0,s1), swap(p0,p1);
		if (max(p0,p1)-eps<min(p0+5,p1+5))
			return 0;
		if (p1>p0+5+eps)
		if ((p1-p0-5)/(s0-s1)<=t1+eps)
			return 0;
	}
	return 1;
}

void dfs(int now){
	ans=max(ans,a[now].t);
	if (now==tot+1){
		ans=1e29;
		return;
	}
	if (p[a[now].x].fi!=p[a[now].y].fi){	
		if (examine(a[now].t,a[now].t))
			dfs(now+1);
		return;
	}
	
	double t0=a[now].t, t1=a[now].t;
	//if (now==tot) t1=1e10;

	p[a[now].x].fi^=1;
	if (examine(t0,t1))
		dfs(now+1);		
	p[a[now].x].fi^=1;
	
	p[a[now].y].fi^=1;
	if (examine(t0,t1))
		dfs(now+1);
	p[a[now].y].fi^=1;
}

int main(){
    freopen("c.in","r",stdin);
    freopen("c.out","w",stdout);
	
	int T; cin>>T;
	Rep(_,1,T){
		scanf("%d",&n);
		Rep(i,1,n){
			char ss[10];
			scanf("%s",ss);
			if (ss[0]=='L') p[i].fi=0; else p[i].fi=1;
			scanf("%d%d",s+i,&p[i].se);
		}
		
		tot=0;
		Rep(i,1,n)
			Rep(j,1,n) if (s[i]>s[j] && p[i].se+5<=p[j].se){
				++tot;
				a[tot].x=i, a[tot].y=j;
				a[tot].t=(p[j].se-p[i].se-5)/(1.0*s[i]-s[j]);
			}
			
		sort(a+1,a+tot+1);
		
		ans=0.0;
		
		dfs(1);
		
		printf("Case #%d: ",_);
		
		if (ans>1e28)
			puts("Possible");
		else	
			printf("%.7lf\n",ans);
		
	}


    return 0;
}
