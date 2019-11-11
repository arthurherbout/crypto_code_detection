#include<cstdio>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<algorithm>
#include<string>
#include<utility>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define ll long long
#define MOD 1000002013

set<int> useInd;
map<int,int> mpInd;

ll get_cost(ll p, ll st, ll ed){
  ll res, dist = ed - st;
  res = dist * (dist + 1) / 2;
  res %= MOD;
  res *= (p % MOD);
  res %= MOD;
  return res;
}

int main(){
  int i, j, k;
  int N, M, o[2000], e[2000], p[2000];
  int st, ed;
  ll in[2000], ind[2000], mx[2000], ok;
  ll bef, aft, res;
  int size, count = 0;

  set<int>::iterator it;

  scanf("%d",&size);
  while(size--){
    scanf("%d%d",&N,&M);
    rep(i,M) scanf("%d%d%d",o+i,e+i,p+i);

    useInd.clear();
    mpInd.clear();
    rep(i,M) useInd.insert(o[i]);
    rep(i,M) useInd.insert(e[i]);

    k = 0;
    for(it=useInd.begin(); it!=useInd.end(); it++){
      mpInd[*it] = k;
      ind[k] = (*it);
      k++;
    }

    N = k;
    rep(i,N) in[i] = 0;

    bef = aft = 0;
    rep(k,M){
      st = mpInd[o[k]];
      ed = mpInd[e[k]];
      REP(i,st,ed) in[i] += p[k];

      bef += get_cost(p[k], o[k], e[k]);
    }

//    puts("in");
//    rep(i,N) printf("%lld ",in[i]); puts("");

    rep(i,N) if(in[i]){
      mx[i] = in[i];
      REP(k,i+1,N) mx[k] = min(mx[k-1], in[k]);

//      printf("%d :", i);
//      REP(k,i,N) printf(" %lld",mx[k]); puts("");

      ok = 0;
      for(k=N-1;k>=i;k--){
        in[k] -= mx[k];
        mx[k] -= ok;
        aft += get_cost(mx[k], ind[i], ind[k+1]);
        ok += mx[k];
      }
    }

    res = aft - bef;
    res %= MOD;
    if(res < 0) res += MOD;
    printf("Case #%d: %lld\n",++count,res);
  }
  

  return 0;
}
