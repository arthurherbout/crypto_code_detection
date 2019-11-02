#include<cstdio>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<algorithm>
#include<string>
#include<utility>
#include<iostream>
#include<cassert>
#include<cmath>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define ll long long

int main(){
  ll i,j,l,x,r;
  int size, count = 0;
  ll R, N, M, K;
  ll p[4] = {2, 3, 5, 7};
  ll in[12][4];
  ll mx[4];

  ll out[12], outs;

  scanf("%d",&size);
  while(size--){
//    fprintf(stderr, "progress %d\n", size);
    printf("Case #%d:\n", ++count);

    scanf("%lld%lld%lld%lld",&R,&N,&M,&K);
    while(R--){
      rep(i,K) rep(j,4) in[i][j] = 0;
      rep(i,K){
        scanf("%lld",&j);
        rep(x,4) while(j%p[x]==0) in[i][x]++, j/=p[x];
      }
      rep(j,4) mx[j] = 0;
      rep(i,K) rep(j,4) mx[j] = max(mx[j], in[i][j]);

      outs = 0;
      rep(i,mx[2]) out[outs++] = p[2];
      rep(i,mx[3]) out[outs++] = p[3];

      if(M <= 5){
        rep(i,mx[1]) out[outs++] = p[1];
        while(mx[0] > N-outs){
          mx[0] -= 2;
          out[outs++] = 4;
        }
        while(outs < N){
          mx[0]--;
          out[outs++] = 2;
        }
      } else {
      }

      rep(i,N) printf("%lld",out[i]); puts("");
    }
  }

  return 0;
}
