#include <iostream>
#include <algorithm>
#include <string>
#include <utility>
#include <map>
using namespace std;

const int MAX_N = 10030;
int E[MAX_N],F[MAX_N];

int Set[MAX_N];

void do_case(){
  int P;
  cin >> P;
  
  for(int i=0;i<P;i++)
    cin >> E[i];
  for(int i=0;i<P;i++)
    cin >> F[i];

  map<int,int> M;
  
  for(int i=0;i<P;i++)
    M[E[i]] = i;
  
  int sum = 0;
  for(int i=0;i<P;i++)
    sum += F[i];
  
  F[M[0]]--; // empty
  sum--;
  int k = 0;
  while(sum > 0){
    for(int i=0;i<P;i++)
      if(F[i] > 0){
        Set[k] = E[i];
        for(int bM=0;bM<(1 << k);bM++){
          int s = Set[k];
          for(int j=0;j<k;j++)
            if((bM >> j) & 1)
              s += Set[j];
          //cout << s << " " << set[k] << endl;
          F[M[s]]--;
          sum--;
        }
        break;
      }
    k++;
  }
  for(int i=0;i<k;i++)
    cout << (i ? " " : "") << Set[i];
  cout << endl;
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
