#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <utility>
#include <set>
#include <cctype>
#include <queue>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <functional>
#include <numeric>

using namespace std;
typedef long long int LL;

LL ipow(LL base, LL p){
  LL ret = 1;
  for(int i = 0; i < p; i++){
    ret *= base;
  }
  return ret;
}

LL T, N, P;
/*
 */

void solve(){
  int n;
  LL gua = 0;
  LL can = 0;
  for(n = 0; n < N; n++){
    if(n != 0){
      can+=ipow(2,N-n);
    }
    if(P < ipow(2,n+1)){
      break;
    }
  }
  if(P == (1<<N)){
    can = (1<<N)-1;
  }

  for(n = 0; n < N; n++){
    if(n != 0){
      gua+=ipow(2,N-n);
    }
    if((1<<N)-P < ipow(2,n+1)){
      break;
    }
  }
  if(P == (1<<N)){
    gua = (1<<N)-1;
  }
  gua = (1<<N) -1 - gua -1;
  if(gua == -1) gua = can;
  cout << gua << ' ' << can;
}

int main(){
  cin >> T;
  for(int t = 0; t < T; t++){
    cin >> N >> P;
    cout << "Case #" << t+1 << ": ";
    solve();
    cout << endl;
  }
  return 0;
}
