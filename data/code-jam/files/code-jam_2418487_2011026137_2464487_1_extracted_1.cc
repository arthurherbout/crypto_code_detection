#include<cstdio>
#include<iostream>
#include<cmath>
#include<map>
#include<set>
#include<string>
#include<vector>
#include<queue>
#include<stack>
#define pb push_back
const double PI=acos(-1);
using namespace std;
typedef long long ll;
    ll t,r;
ll ef(ll n0,ll n){
    //cout<<n0<<" "<<n<<endl;
    if(n0==n)
        return n;
    ll m=(n0+n)/2;
    m++;
    //cout<<m<<" "<<2*m*m+(2*r-1)*m<<endl;
    if(2*m*m+(2*r-1)*m<=t)
        return ef(m,n);
    return ef(n0,m-1);
}
int main(){
    freopen("in","r",stdin);
    freopen("out","w",stdout);
    int i,j,k,n,m,x,z,y,T;
    ll ans=1000000000ll;
    scanf("%d",&T);
    for(int ii=1;ii<=T;ii++){
        printf("Case #%d: ",ii);
        cin>>r>>t;
        cout<<ef(0,min(ans,ans*ans/(2*r-1)))<<endl;
    }
    return 0;
}
