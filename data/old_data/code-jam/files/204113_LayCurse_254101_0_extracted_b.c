#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define INF 1000000000

int x,y,r;
char mp[59][58];

int dp[12][12][600][600];

int solve(int a,int b,int fg1,int fg2){
  int i,j,k,mn,mx,res=INF,tmp,fal;
  int c1=a,c2=b,c3=fg1,c4=fg2;

  if(dp[c1][c2][c3][c4]>=0) return dp[c1][c2][c3][c4];
  if(a==x-1) return dp[c1][c2][c3][c4]=0;

  fal=0;
  if(mp[a+1][b]=='.' || (fg2&(1<<b))){
    a++; fg1=fg2; fg2=0; fal++;
    while( mp[a+1][b]=='.' || (fg2&(1<<b)) ){
      a++; fg1=fg2; fg2=0; fal++;
    }
    if(fal>r) return dp[c1][c2][c3][c4]=res;
    return dp[c1][c2][c3][c4]=solve(a,b,fg1,fg2);
  }

  mn=b; mx=b;
  while(mn-1>=0 && (mp[a][mn-1]=='.'||(fg1&(1<<(mn-1)))) && mp[a+1][mn-1]=='#' && !(fg2&(1<<(mn-1)))) mn--;
  while(mx+1< y && (mp[a][mx+1]=='.'||(fg1&(1<<(mx+1)))) && mp[a+1][mx+1]=='#' && !(fg2&(1<<(mx+1)))) mx++;
  
  if(mn-1>=0 && (mp[a][mn-1]=='.'||(fg1&(1<<(mn-1))))){
    tmp = solve(a,mn-1,fg1,fg2);
    if(res>tmp) res=tmp;
  }
  if(mx+1<y  && (mp[a][mx+1]=='.'||(fg1&(1<<(mx+1))))){
    tmp = solve(a,mx+1,fg1,fg2);
    if(res>tmp) res=tmp;
  }
  
  REP(i,mn,mx+1){
    if(i-1>=0 && mp[a+1][i-1]=='#' && !(fg2&(1<<(i-1)))){
      tmp = solve(a,i,fg1,fg2|(1<<(i-1)))+1;
      if(res>tmp) res=tmp;
    }
    if(i+1< y && mp[a+1][i+1]=='#' && !(fg2&(1<<(i+1)))){
      tmp = solve(a,i,fg1,fg2|(1<<(i+1)))+1;
      if(res>tmp) res=tmp;
    }
  }

  return dp[c1][c2][c3][c4]=res;
}

int main(){
  int i,j,k,l,m,n;
  int size,count=0;

  scanf("%d",&size);
  while(size--){
    scanf("%d%d%d",&x,&y,&r);
    rep(i,x) scanf("%s",mp[i]); rep(j,y) mp[x][j]='#';
    rep(i,x) rep(j,y) rep(k,1<<y) rep(m,1<<y) dp[i][j][k][m]=-1;
    m=solve(0,0,0,0);
    printf("Case #%d: ",++count);
    if(m==INF) puts("No"); else printf("Yes %d\n",m);
  }

  return 0;
}
