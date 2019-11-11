#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

int N, A[2000];

int main(){
  int i, j, k;
  int mn;
  int T, cnt = 0;
  int res;

  scanf("%d",&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);

    scanf("%d",&N);
    rep(i,N) scanf("%d",A+i);
    res = 0;

    while(N){
      mn = 0;
      REP(i,1,N) if(A[mn] > A[i]) mn = i;
      res += min(mn, N-1-mn);
      N--;
      REP(i,mn,N) A[i] = A[i+1];
    }

    printf("%d\n",res);
  }

  return 0;
}
