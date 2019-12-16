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

class FenwickTree{  // All entries must be >= 0 even after decrement
public:             // Every function is O(log n)
  FenwickTree(int n) : N(n), iBM(1), tree(n,0) {
    while (iBM < N) iBM *= 2;
  }
  
  // inc/dec the entry at position idx by val
  void incEntry(int idx, int val) {
    do tree[idx] += val; while(idx && (idx += (idx & (-idx))) < N);
  }
  
  // return the cumulative sum val[0] + val[1] + ... + val[idx]
  int cumulativeSum(int idx) const {
    int sum = tree[0];
    for( ; idx > 0 ; idx &= idx-1) sum += tree[idx];
    return sum;
  }
  
  // return the entry indexed by idx
  int getEntry(int idx) const {
    int val = tree[idx], par = idx & (idx-1);
    if (idx--) for( ; par != idx ; idx &= idx-1) val -= tree[idx];
    return val;
  }
  
  // return the largest index such that the cumulative frequency is
  // what is given, or -1 if it is not found
  int getIndex(int sum) const {
    if ((sum -= tree[0]) < 0) return -1;
    int idx = 0;
    for(int bM = iBM ; bM != 0 && idx < N-1 ; bM >>= 1)
      if (sum >= tree[idx+bM]) sum -= tree[idx += bM];
    return (sum != 0) ? -1 : min(N-1,idx);
  }
  
private:
  int N, iBM; vector<int> tree;
};

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
  
  mini[0] = maxi[0] = S[0];
  for(int i=1;i<n;i++){
    mini[i] = mini[M[i]];
    maxi[i] = maxi[M[i]];
    if(S[i] < mini[i]) mini[i] = S[i];
    if(S[i] > maxi[i]) maxi[i] = S[i];
  }
  
  //f(0,-1,-1);
  
  FenwickTree F(2*MAX_N);
  
  for(int i=0;i<n;i++){
    int lo = maxi[i]-D, hi = mini[i];
    if(lo < 0) lo = 0;
    
    if(lo <= hi){
      F.incEntry(lo,1);
      F.incEntry(hi+1,-1);
    }
  }
  
  int best = 0;
  if(S[0] <= D) best = F.cumulativeSum(0);
  
  for(int lo=max(1ll,S[0]-D);lo<=S[0];lo++){
    int tmp = F.cumulativeSum(lo);
    if(tmp > best) { best = tmp; }
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
