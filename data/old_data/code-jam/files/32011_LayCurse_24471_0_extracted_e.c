#include<stdio.h>
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

int x,y;
int dp[100000], next[100000];
char mp[100][100];

int lst[100000], sz;

int main(){
  int i,j,k,l,m,n,r,p,tmp,res,pp,mx;
  int size,count=0;
  int use[100];


  scanf("%d",&size);
  while(size--){
    fprintf(stderr,"rest input = %d\n",size);

    scanf("%d%d",&x,&y);
    rep(i,x) scanf("%s",mp[i]);
    rep(i,y) mp[x][i]='.'; x++;
    
    rep(i,(1<<y)) dp[i]=-1000000000; dp[0]=0;

    rep(r,x){
      rep(i,(1<<y)) next[i] = -1000000000;

      mx=0; rep(i,(1<<y)) if(mx<dp[i]) mx=dp[i];
      sz=0; rep(i,(1<<y)) if(dp[i]>mx-6) lst[sz++]=i;
      
      rep(k,(1<<y)){
        rep(j,y) if(k&(1<<j)) use[j]=1; else use[j]=0;
        rep(j,y) if( (use[j]&&mp[r][j]=='.') || (use[j]==0&&mp[r][j]=='#') ) break;
        if(j<y) continue;

        rep(pp,sz){
//        rep(p,(1<<y)) if(dp[p] >= 0){
          p=lst[pp];
          tmp = dp[p];
          rep(j,y) if(p&(1<<j)) if(use[j]==0) tmp++;
          rep(j,y) if(use[j]){
            tmp += 3;
            if(j && use[j-1]) tmp--;
            if(j<y-1 && use[j+1]) tmp--;
            if(p&(1<<j)) tmp--;
          }
          if(next[k] < tmp) next[k] = tmp;
        }
      }

      rep(i,(1<<y)) dp[i]=next[i];
    }

    res=dp[0];
    printf("Case #%d: %d\n",++count,res);
  }

  return 0;
}
