#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <tr1/tuple>
using namespace std;
using namespace std::tr1;

#define ll long long
typedef vector<int> vi;
map<vi,ll> M;

const int MAX_N = 4;

int G[MAX_N],P,Q,N;

ll f(vi v){
  if(M.count(v)) return M[v];
  
  ll ans = 0;
  for(int i=0;i<=N;i++){
    if(i < N && v[i] <= 0) continue;
    if(i < N)
      v[i] -= P;
    int tower = -1;
    for(int j=0;j<N;j++)
      if(v[j] > 0){
	tower = j;
	break;	
      }
    ll curr = 0;
    if(i < N && v[i] <= 0) curr += G[i];
    if(tower != -1){
      v[tower] -= Q;
      curr += f(v);
      v[tower] += Q;
    }
    if(curr > ans) ans = curr;
    if(i < N) v[i] += P;
  }
  
  return M[v] = ans;
}

void do_case(){
  M.clear();
  cin >> P >> Q >> N;
  vi v(N);
  for(int i=0;i<N;i++) cin >> v[i] >> G[i];
  
  cout << f(v) << endl;
}

int main(){
  int T,C=1;
  cin >> T;
  while(T--){
    cout << "Case #" << C++ << ": ";
    do_case();
  }
  
  return 0;
}
