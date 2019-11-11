#include <iostream>
#include <string>
#include <cmath>
#include <vector>
using namespace std;
int T;
int E, R, N;
int dfs(vector<int> val, int index, int ene){
  if(index == val.size()-1){
    return ene*val[index];
  }
  int tmpVal = -1;
  int bestVal = -1;
  
  for(int e = 0; e <= ene; e++){
    tmpVal = e*val[index] + dfs(val,index+1,min(E,ene-e+R));
    if(bestVal < tmpVal){
      bestVal = tmpVal;
    }
  }
  return bestVal;
}


int main(){
  

  cin >> T;
  for(int t = 0; t < T; t++){

    cin >> E >> R >> N;
    vector<int> num(N);
    for(int n = 0; n < N; n++){
      cin >> num[n];
    }
    cout << "Case #" << t+1 << ": ";
    // do the task ->
    // consume 
    // use at least E-R joules
    
    cout << dfs(num,0,E) << endl;

  }
}
