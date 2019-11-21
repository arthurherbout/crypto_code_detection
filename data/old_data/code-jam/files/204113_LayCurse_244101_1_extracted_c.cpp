#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<algorithm>
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)
using namespace std;

int n,mx,opt;
int edge[120][120];
int ok[120][120];
int ind[120];
int cou;

void solve(int d){
  int i,j,k,fg=0;

  if(mx>=opt) return;
  if(d==n){opt=mx; return;}

  if(cou++ > 2000) return;

  rep(i,mx+1) if(ok[ind[d]][i]==0){
    if(i==mx) fg=1; else fg=0;
    REP(j,d+1,n) if(edge[ind[d]][ind[j]]) ok[ind[j]][i]++;
    if(fg) mx++;
    solve(d+1);
    if(fg) mx--;
    REP(j,d+1,n) if(edge[ind[d]][ind[j]]) ok[ind[j]][i]--;
  }
  
}

int main(){
  int i,j,k,l,m,K,loop;
  int in[120][120];
  int size,count=0;

  long long a,b;

  scanf("%d",&size);
  while(size--){
    fprintf(stderr,"rest %d\n",size);
    scanf("%d%d",&n,&K);
    rep(i,n) rep(j,K) scanf("%d",in[i]+j);

    rep(i,n) rep(j,n) edge[i][j]=0;
    REP(k,1,K){
      rep(i,n) REP(j,i+1,n){
        a=in[i][k-1]-in[j][k-1];
        b=in[i][k  ]-in[j][k  ];
        if(a*b<=0) edge[i][j]=edge[j][i]=1;
      }
    }

    opt=n;
    rep(loop,2000){
      rep(i,n) ind[i]=i;
      random_shuffle(ind,ind+n);
      
      mx=0; cou=0;
      rep(i,n) rep(j,n) ok[i][j]=0;
      
      solve(0);
    }

    printf("Case #%d: %d\n",++count,opt);
  }

  return 0;
}
