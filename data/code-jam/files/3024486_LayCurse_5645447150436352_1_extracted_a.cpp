#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define ll long long

int N, P, Q, R, S;
ll A[2000000], sum[2000000];;

int main(){
  int i, j, k;
  int T, cnt = 0;
  ll val;
  double res, tmp;

  scanf("%d",&T);
  while(T--){
    //fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);

    scanf("%d%d%d%d%d",&N, &P,&Q,&R,&S);
    rep(i,N) A[i] = ((ll)i*P+Q) % R + S;
    sum[0] = 0;
    rep(i,N) sum[i+1] = sum[i] + A[i];

    res = 0;
    j = 0;
    rep(i,N){
      while( j+1<N && max(sum[N]-sum[j+1], sum[j+1]-sum[i]) > max(sum[N]-sum[j+2], sum[j+2]-sum[i]) ) j++;
      val = max(sum[i], max(sum[N]-sum[j+1], sum[j+1]-sum[i]));
      tmp = (sum[N] - val) / (double)sum[N];
      res = max(res, tmp);
    }

    printf("%.10f\n",res);
  }

  return 0;
}
