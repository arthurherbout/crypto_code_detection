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

ll K, r;
ll res[12], cur[12], v[12];
double mxp;
double comb[20][20], pw[20];
ll mx[4];

void solve(int depth, int mn){

  if(depth==r){
    int i, j, k; ll val;
    double p[12] = {}, pr;

    val = 1;
    rep(i,depth) val *= cur[i];
    rep(i,mx[0]){
      if(val%2) return;
      val /= 2;
    }
    rep(i,mx[1]){
      if(val%3) return;
      val /= 3;
    }

    rep(i,1<<depth){
      val = 1;
      rep(j,depth) if(i&1<<j) val *= cur[j];
      rep(j,K) if(val==v[j]) p[j]+=1;
    }
    pr = 1;
    rep(j,K) pr *= p[j];

    if(pr > mxp){
      mxp = pr;
      rep(i,r) res[i] = cur[i];
    }
    
    return;
  }

  if(mn <= 2){ cur[depth] = 2; solve(depth+1, 2); }
  if(mn <= 3){ cur[depth] = 3; solve(depth+1, 3); }
  if(mn <= 4){ cur[depth] = 4; solve(depth+1, 4); }
  if(mn <= 6){ cur[depth] = 6; solve(depth+1, 6); }
  if(mn <= 8){ cur[depth] = 8; solve(depth+1, 8); }
}

int main(){
  ll i,j,l,x;
  int size, count = 0;
  ll R, N, M;
  ll p[4] = {2, 3, 5, 7};
  ll in[12][4];

  ll out[12], outs;

  rep(i,20) comb[0][i] = 0;
  rep(i,20) comb[i][0] = 1;
  REP(i,1,20) REP(j,1,20) comb[i][j] = (comb[i-1][j] + comb[i-1][j-1]);

  pw[0] = 1;
  REP(i,1,20) pw[i] = pw[i-1] / 2;

  scanf("%d",&size);
  while(size--){
    printf("Case #%d:\n", ++count);

    scanf("%lld%lld%lld%lld",&R,&N,&M,&K);
    while(R--){
      fprintf(stderr, "progress %d\n", R);
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
        r = N - outs;

        rep(i,K){
          v[i] = 1;
          rep(j,in[i][0]) v[i] *= p[0];
          rep(j,in[i][1]) v[i] *= p[1];
        }

        mxp = -1;
        solve(0, 0);

        rep(i,r) out[outs++] = res[i];
      }

      rep(i,N) printf("%lld",out[i]); puts("");
    }
  }

  return 0;
}
