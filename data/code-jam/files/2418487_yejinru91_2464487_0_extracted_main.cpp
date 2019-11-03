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

int main(){

	freopen("A-small-attempt0.in","r",stdin);
	freopen("sum.out","w",stdout);


    int ncase;
    cin>>ncase;
    rep1(i,ncase){
        printf("Case #%d: ",i);
        ll ra,tot;
        cin>>ra>>tot;
        ll l = 1 , r = ll( sqrt(tot*1.0) );
        ll ans = 0;
        while(l<=r){
            ll n = (l+r)/2;
            ll a = (n-1);
            ll b = ra;
            ll tmp = (tot-n)/2/n;
            //cout<<"n = "<<n<<" "<<n*n<<endl;

            if( a<=tmp-b ){
                ans = n;
                l = n+1;
            }else
                r = n-1;
        }
        cout<<ans<<endl;
    }

	return 0;
}
