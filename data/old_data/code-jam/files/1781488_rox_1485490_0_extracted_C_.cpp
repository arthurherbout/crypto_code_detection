#include <cstdio>
#include <algorithm>
#include <cstring>
#include <map>
#include <vector>
using namespace std;
typedef long long ll;
int n,m;
ll a[111],b[111];
int A[111],B[111];
ll solve()
{
  ll res=0,t;
  t=0;
  for(int i=0;i<m;i++){
    ll p1[111],c1=0;
    for(int ii=0;ii<=i;ii++){
      if(A[0]==B[ii]){
        ll v=min(a[0],b[ii]);
        p1[c1++]=v;
        a[0]-=v; b[ii]-=v;
        t+=v;
      }
    }
    res=max(res,t);
    if(n==1)goto N1;
    for(int j=i;j<m;j++){
      ll p[111],c=0;
      for(int ii=i;ii<=j;ii++){
        if(A[1]==B[ii]){
          ll v=min(a[1],b[ii]);
          p[c++]=v;
          a[1]-=v; b[ii]-=v;
          t+=v;
        }
      }
      res=max(res,t);
      if(n==2)goto N2;
      for(int k=j;k<m;k++){
        ll p[111],c=0;
        for(int ii=j;ii<=k;ii++){
          if(A[2]==B[ii]){
            ll v=min(a[2],b[ii]);
            p[c++]=v;
            a[2]-=v; b[ii]-=v;
            t+=v;
          }
        }
        res=max(res,t);
        for(int ii=k;ii>=j;ii--){
          if(A[2]==B[ii]){
            ll v=p[--c];
            a[2]+=v; b[ii]+=v;
            t-=v;
          }
        }
      }
    N2:;
      for(int ii=j;ii>=i;ii--){
        if(A[1]==B[ii]){
          ll v=p[--c];
          a[1]+=v; b[ii]+=v;
          t-=v;
        }
      }
    }
  N1:;
    for(int ii=i;ii>=0;ii--){
      if(A[0]==B[ii]){
        ll v=p1[--c1];
        a[0]+=v; b[ii]+=v;
        t-=v;
      }
    }
  }
  return res;
}
int main(void)
{
  int t; scanf("%d",&t);
  for(int T=1;T<=t;T++){
    printf("Case #%d: ",T);
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++) scanf("%lld%d",a+i,A+i);
    for(int i=0;i<m;i++) scanf("%lld%d",b+i,B+i);
    printf("%lld\n",solve());
  }
}
