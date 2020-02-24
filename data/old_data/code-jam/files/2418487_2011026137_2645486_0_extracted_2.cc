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
int a[1009];
vector<int>v;
int ans=0,n,r,E,exr;
void dfs(int x,int e,int m){
    e+=exr;
    exr=0;
    if(e>E){
        exr+=E-e;
        e=E;
    }
    if(x==n+1){
        ans=max(m,ans);
        return;
    }
    for(int i=0;i<=e;i++)
        dfs(x+1,e-i+r,m+a[x]*i);
}
int main(){
    freopen("in","r",stdin);
    freopen("out","w",stdout);
    int i,j,k,x,z,y,T,e;
    scanf("%d",&T);
    for(int ii=1;ii<=T;ii++){
        printf("Case #%d: ",ii);
        cin>>E>>r>>n;
        r=min(E,r);
        for(i=1;i<=n;i++)
            scanf("%d",&a[i]);
        ans=0;
        exr=0;
        dfs(1,E,0);
        cout<<ans<<endl;
    }
    return 0;
}
