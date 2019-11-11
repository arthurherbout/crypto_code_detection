/* Writen by Filip Hlasek 2012 */
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <cmath>
#include <cstring>

#define FOR(i,a,b) for(int i=(a);i<=(b);i++)
#define FORD(i,a,b) for(int i=(a);i>=(b);i--)
#define REP(i,b) for(int i=0;i<(b);i++)

using namespace std;

#define MAXN 111
int A[MAXN], B[MAXN], C[MAXN], o[MAXN];
int _A[MAXN], _B[MAXN];
int N, K;
#define ld long double
ld dp[MAXN][MAXN][2];

bool cmp(int a,int b){ return (long long)A[a] * B[b] > (long long)A[b] * B[a]; }

int main(int argc, char *argv[]){
  int T;
  scanf("%d",&T);
  REP(t,T){
    int _N;
    scanf("%d%d",&_N,&K);
    REP(i,_N) scanf("%d/%d %d",_A+i,_B+i,C+i);
    N = 0;
    REP(i,_N) REP(j,C[i]){
      A[N] = _A[i];
      B[N] = _B[i];
      N++;
    }
    REP(i,N) o[i] = i;
    sort(o, o + N, cmp);

    REP(i,N+1) REP(j,K+1) dp[i][j][0] = dp[i][j][1] = 0;
    dp[0][0][1] = 1;
    REP(i,N) REP(j,K+1){
      ld p = (ld)A[o[i]] / B[o[i]];
      //printf("i:%d j:%d 0:%Lf 1:%Lf\n",i,j,dp[i][j][0],dp[i][j][1]);
      ld ans0 = dp[i][j][0], ans1 = dp[i][j][1];
      int konst = K - j + 1;
      if(ans0 + ans1 * konst > dp[i+1][j][0] + dp[i+1][j][1] * konst){
        dp[i+1][j][0] = ans0;
        dp[i+1][j][1] = ans1;
      }
      dp[i+1][j+1][0] = (1-p) * (ans0 + ans1);
      dp[i+1][j+1][1] = p * ans1;
    }
    ld ans = dp[N][K][0] + dp[N][K][1];
    
    //ld ans = ans0 + ans1;
    printf("Case #%d: %.9Lf\n", t+1, 1 - ans);
  }
  return 0;
}
