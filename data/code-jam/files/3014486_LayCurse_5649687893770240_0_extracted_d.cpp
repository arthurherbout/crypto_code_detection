#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

int M, N;
int val[10];
int res1, res2;
string S[10];

void solve(int depth){
  int i, j, k, tmp;
  set<string> s;

  if(depth==M){
    rep(i,N){
      rep(j,M) if(val[j]==i) break;
      if(j==M) return;
    }
    tmp = 0;
    rep(i,N){
      s.clear();
      rep(j,M) if(val[j]==i){
        rep(k,S[j].size()+1) s.insert(S[j].substr(0,k));
      }
      tmp += s.size();
    }
    if(res1 < tmp) res1 = tmp, res2 = 0;
    if(res1==tmp) res2++;
    return;
  }
  
  rep(i,N){
    val[depth] = i;
    solve(depth+1);
  }
}

int main(){
  int i, j, k;
  int T, cnt = 0;
  char buf[100];

  scanf("%d",&T);
  while(T--){
    fprintf(stderr,"rest %d\n",T);
    printf("Case #%d: ", ++cnt);

    scanf("%d%d",&M,&N);
    rep(i,M) scanf("%s",buf), S[i]=buf;
    res1 = 0;
    solve(0);

    printf("%d %d\n", res1, res2);
  }

  return 0;
}
