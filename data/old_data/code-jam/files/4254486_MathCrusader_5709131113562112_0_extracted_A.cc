#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
using namespace std;

const int MAX_N = 1000030;
long long S[MAX_N],M[MAX_N];

vector<int> adj[MAX_N];

int mini[MAX_N],maxi[MAX_N];

void f(int v,int lo,int hi){
  if(v == 0)
    hi = lo = mini[v] = maxi[v] = S[v];
  
  if(S[v] < lo) lo = S[v];
  if(S[v] > hi) hi = S[v];
  
  mini[v] = lo;
  maxi[v] = hi;
  
  for(int i=0;i<adj[v].size();i++)
    f(adj[v][i],lo,hi);
}

void do_case(){
  int n,D;
  cin >> n >> D;
  
  long long As,Cs,Rs;
  cin >> S[0] >> As >> Cs >> Rs;
  
  long long Am,Cm,Rm;
  cin >> M[0] >> Am >> Cm >> Rm;
  
  for(int i=1;i<n;i++){
    S[i] = (S[i-1]*As + Cs) % Rs;
    M[i] = (M[i-1]*Am + Cm) % Rm;
  }
  
  for(int i=1;i<n;i++)
    M[i] %= i;
  
  for(int i=0;i<n;i++)
    adj[i].clear();
  
  for(int i=1;i<n;i++)
    adj[M[i]].push_back(i);
  
  f(0,-1,-1);
  
  int best = 0;
  for(int lo=max(0ll,S[0]-D);lo<=S[0];lo++){
    int tmp = 0;
    for(int i=0;i<n;i++)
      if(lo <= mini[i] && maxi[i] <= lo+D)
        tmp++;
    if(tmp > best) best = tmp;
  }
  cout << best << endl;
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
