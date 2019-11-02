#include<cstdio>
#include<iostream>
#include<cmath>
#include<map>
#include<set>
#include<algorithm>
#include<string>
#include<vector>
#include<queue>
#include<stack>
#define pb push_back
const double PI=acos(-1);
using namespace std;
typedef long long ll;
int a[10000009];
vector<int>v;
int main(){
    freopen("in","r",stdin);
    freopen("out","w",stdout);
    int i,j,k,n,m,x,z,y,T,e,r;
    scanf("%d",&T);
    for(int ii=1;ii<=T;ii++){
        printf("Case #%d: ",ii);
        cin>>e>>r>>n;
        for(i=1;i<=n;i++)
            scanf("%d",&a[i]);
        int m=0;
        for(i=n;i>=1;i--)
            if(a[i]>=m){
                m=a[i];
                v.pb(i);
            }
        v.pb(0);
        ll ans=0;
        reverse(v.begin(),v.end());
        for(i=0;i<v.size()-1;i++){
            for(j=v[i]+1;j<v[i+1];j++){
                ans+=min(e,r)*a[j];
            }
            ans+=ll(e)*a[j];
            e=r;
        }
        v.clear();
        cout<<ans<<endl;
    }
    return 0;
}
