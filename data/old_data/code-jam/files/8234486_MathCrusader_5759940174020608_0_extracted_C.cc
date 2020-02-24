#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <map>
#include <sstream>
#include <vector>
using namespace std;

const int MAX_N = 10030;
string A[MAX_N];
vector<int> B[MAX_N];

void do_case(){
  int N;
  cin >> N;
  
  getline(cin,A[0]);
  for(int i=0;i<N;i++){
    getline(cin,A[i]);
    B[i].clear();
  }
  
  map<string,int> w;
  
  for(int i=0;i<N;i++){
    stringstream ss(A[i]);
    string s;
    while(ss >> s){
      if(w.count(s) == 0){
        int x = w.size();
        w[s] = x;
      }
      B[i].push_back(w[s]);
    }
  }
  
  vector<int> M; M.resize(w.size());
  
  for(int i=0;i<B[0].size();i++)
    M[B[0][i]] |= 1;
  for(int i=0;i<B[1].size();i++)
    M[B[1][i]] |= 2;
  
  
  
  for(int i=2;i<N;i++){
    A[i-2] = A[i];
    B[i-2] = B[i];
  }
  N -=2;
  
  int best = 100000;
  
  for(int b=0;b<(1 << N);b++){
    vector<int> X = M;
    for(int i=0;i<N;i++){
      int val = ((b >> i) & 1) ? 1 : 2;
      for(int j=0;j<B[i].size();j++)
        X[B[i][j]] |= val;
    }
    int ctr = 0;
    for(int i=0;i<X.size();i++)
      if(X[i] == 3)
        ctr++;
    if(ctr < best) best = ctr;
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
