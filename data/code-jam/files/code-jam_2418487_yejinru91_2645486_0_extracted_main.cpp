#include <set>
#include <map>
#include <cmath>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

#define lx(x) (x<<1)
#define rx(x) (x<<1|1)
#define debug puts("here")
#define rep(i,n) for(int i=0;i<n;i++)
#define rep1(i,n) for(int i=1;i<=n;i++)
#define REP(i,a,b) for(int i=a;i<=b;i++)
#define foreach(i,vec) for(unsigned i=0;i<vec.size();i++)
#define pb push_back
#define RD(n) scanf("%d",&n)
#define RD2(x,y) scanf("%d%d",&x,&y)
#define RD3(x,y,z) scanf("%d%d%d",&x,&y,&z)
#define RD4(x,y,z,w) scanf("%d%d%d%d",&x,&y,&z,&w)

/******** program ********************/

const int MAXN = 1e4+5;

int val[MAXN];
int n,r,e;
int ans;

int find_pos(int l,int r){
    int pos = l;
    REP(i,l,r)
        if(val[pos]<val[i])
            pos = i;
    return pos;
}

void dfs(int x,int res,int now){
    if(x>n){
        ans = max(ans,now);
        return;
    }
    for(int i=res;i>=0;i--)
        dfs( x+1,min(i+r,e),(res-i)*val[x]+now );
}

int main(){


	freopen("B-small-attempt0.in","r",stdin);
	freopen("sum.out","w",stdout);


    int ncase;
    RD(ncase);
    rep1(Ncase,ncase){
        printf("Case #%d: ",Ncase);
        RD3(e,r,n);
        rep1(i,n)
            RD(val[i]);
        ans = 0;
        dfs(1,e,0);
        cout<<ans<<endl;
    }

	return 0;
}
