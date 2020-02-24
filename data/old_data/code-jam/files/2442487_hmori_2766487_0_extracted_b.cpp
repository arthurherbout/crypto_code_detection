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

int ipow(int base, int p){
  int ret = 1;
  for(int i = 0; i < p; i++){
    ret *= base;
  }
  return ret;
}

int T, N, P;
/*
 */
#define MAX_N 10
int bestR[1<<MAX_N];
int worstR[1<<MAX_N];

void solve(){
  bestR[0] = 0;
  int c = 1;
  for(int n = 1; n < N; n++){
    for(int i = 0; i < 1<<(N-n); i++, c++){
      bestR[c] = ipow(2,n)-1;
    }
  }
  bestR[c] = (1<<N)-1;
  for(int i = 0; i < (1<<N); i++){
    worstR[(1<<N)-i-1] = (1<<N)-1-bestR[i];
  }
  /*
  for(int i = 0; i < 1<<N; i++){
    
    if(i == 0 || i == (1<<N)-1){
      bestR[i] = i;
    }else if(i <= 1<<(N-1)){
      bestR[i] = 1;
    }else{
      bestR[i] = (i+1)/2;
    }
    

    worstR[(1<<N)-i-1] = (1<<N)-1-bestR[i];
  }
  */
  int can=0, gur=0;
  for(int i = 0; i < 1<<N; i++){
    if(bestR[i] < P){
      can=i;
    }
    if(worstR[i] < P){
      gur=i;
    }
    //cout << bestR[i] << ' ' << worstR[i] << endl;
  }
  cout << gur << ' ' << can;
}

int main(){
  cin >> T;
  for(int t = 0; t < T; t++){
    cin >> N >> P;
    
    /*
     */
    cout << "Case #" << t+1 << ": ";
    solve();
    cout << endl;
  }
  return 0;
}
