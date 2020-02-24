#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
const int MAX_N = 10;    // Change as necessary
const ll  MODD = 1000000009; //

int A[MAX_N][MAX_N];

bool f(int bM,int machine,int person,int n){
  if(person == 0) return true;
  //cout << person << " " << machine << endl;
  
  for(int i=0;i<n;i++)
    if((person >> i) & 1){
      int ctr =0;
      for(int j=0;j<n;j++)
        if(((machine >> j) & 1) && ((bM >> (i*n+j))&1)){
          ctr++;
          if(!f(bM,(machine^(1 << j)),(person ^ (1 << i)),n))
            return false;
        }
      if(ctr == 0) return false;
    }
  return true;
}

void do_case(){
  int n; cin >> n;
  int bb = 0,cc = 0;
  for(int i=0;i<n;i++)
    for(int j=0;j<n;j++){
      char c; cin >> c;
      A[i][j] = (c == '1');
      if(!A[i][j])
        bb |= (1 << (i*n+j));
      else
        cc |= (1 << (i*n+j));
    }
  
  int best = n*n;
  for(int i=0;i<(1 << (n*n));i++){
    if(__builtin_popcount(i) >= best) continue;
    if(f((i|cc),(1 << n)-1,(1 << n)-1,n))
      best = __builtin_popcount(i);
  }
  cout << best << endl;
}

int main(){
  ios_base::sync_with_stdio(false);
  cin.tie(0);
  
  int T,C=1; cin >> T;
  
  while(T--){
    cout << "Case #" << C++ << ": ";
    do_case();
  }
  
  return 0;
}
