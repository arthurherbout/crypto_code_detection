#include <iostream>
#include <string>
#include <iomanip>
#include <set>
using namespace std;

#define ll long long

const ll oo = 1000000000000000ll;
const int MAX_N = 1000030;
ll A[MAX_N];
ll C[MAX_N];

void do_case(){
  ll N,p,q,r,s;
  cin >> N >> p >> q >> r >> s;
  
  for(int i=0;i<N;i++) A[i] = (i*p+q) % r+s;
  C[0] = A[0];
  for(int i=1;i<N;i++) C[i] = A[i] + C[i-1];
  
  ll tot = C[N-1];  
  ll best = oo;
  for(int i=0;i<N;i++){
    ll l_sum = C[i]; ll r_sum = tot-C[i];
    ll maxi = max(l_sum,r_sum);
    if(maxi < best) best = maxi;
  }
  
  
  ll r_sum = 0;;
  for(int r=N-1;r>=0;r--){
    r_sum += A[r];
    int ind = lower_bound(C,C+r,(tot-r_sum)/2) - C;
    int test[3] = {(ind > 0 ? ind-1 : ind),ind,(ind+1 < r ? ind+1 : ind)};
    for(int i=0;i<3;i++){
      ll l_sum = C[test[i]];
      ll m_sum = tot - r_sum - l_sum;
      ll maxi = max(l_sum,max(r_sum,m_sum));
      if(maxi < best) best = maxi;
    }
  }
  long double ONE = 1.0;
  cout << 1 - best / ONE / tot << endl;
}

int main(){
  cout << fixed << setprecision(12);
  int T,C=1;
  cin >> T;
  while(T--){
    cout << "Case #" << C++ << ": ";
    do_case();
  }
  
  return 0;
}
