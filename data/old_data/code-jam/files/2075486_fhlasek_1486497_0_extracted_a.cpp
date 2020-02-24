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
int N;
int X[MAXN], Y[MAXN], M[MAXN];
vector<pair<int,int> > dp[MAXN][1001]; //from, dp

vector< pair<int,int> > states;
bool cmp(pair<int,int> a, pair<int,int> b){ 
  if(M[a.first] + a.second == M[b.first] + b.second) return a.first > b.first;
  return M[a.first] + a.second < M[b.first] + b.second;
}

int main(int argc, char *argv[]){
  int T;
  scanf("%d",&T);
  REP(t,T){
    scanf("%d",&N);
    REP(i,N) scanf("%d%d%d",X+i,Y+i,M+i);
    X[N] = 0; Y[N] = 0; M[N] = 0;
    N++;
    states.clear();
    REP(i,N) REP(t,1001) states.push_back(make_pair(i,t));
    sort(states.begin(),states.end(),cmp);
    REP(i,N) REP(t,1001) dp[i][t].clear();
    dp[N-1][0].push_back(make_pair(N-1,0));
    int ans = 0;
    REP(state, states.size()){
      int i = states[state].first, t = states[state].second;
      REP(d,dp[i][t].size()){
        ans = max(ans, dp[i][t][d].second);
        //printf("i:%d t:%d d:%d dp:%d prev:%d\n",i,t,d,dp[i][t][d].second,dp[i][t][d].first);
        REP(j,N) if(i != j && j != dp[i][t][d].first){
          int arrive = M[i] + t + max(750, 100 * max(abs(X[i] - X[j]), abs(Y[i] - Y[j])));
          if(i == N-1) arrive = M[i] + t + 100 * max(abs(X[i] - X[j]), abs(Y[i] - Y[j]));
          if(arrive > M[j] + 1000) continue;
          if(arrive < M[j]) arrive = M[j];
          //printf("i:%d j:%d t:%d dp:%d arrive:%d\n",i,j,t,dp[i][t],arrive);
          int tt = arrive - M[j];
          dp[j][tt].push_back(make_pair(i, dp[i][t][d].second + 1));
        }
      }
    }
    printf("Case #%d: %d\n",t+1, ans);
  }
  return 0;
}
