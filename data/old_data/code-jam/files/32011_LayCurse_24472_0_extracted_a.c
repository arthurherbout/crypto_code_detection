#include<stdio.h>
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

int sum[10001][10001];

int main(){
  int i,j,k,l,m,n;
  int a,b,c;
  int size,count=0;
  int res;

  scanf("%d",&size);
  while(size--){
    fprintf(stderr,"rest input = %d\n",size);

    rep(i,10001) rep(j,10001) sum[i][j]=0;
    scanf("%d",&n);
    while(n--){
      scanf("%d%d%d",&a,&b,&c);
      REP(i,a,10001) REP(j,b,10001){
        if(10000 - i - j < c) break;
        sum[i][j]++;
      }
    }
    res=0;
    rep(i,10001) rep(j,10001) if(res < sum[i][j]) res=sum[i][j];
    printf("Case #%d: %d\n",++count,res);
  }

  return 0;
}
