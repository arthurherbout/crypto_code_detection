#include<iostream>

using namespace std;

const static int N = 3;
const static int M = 5;
const static int K = 7;
int sets[K];
int buf[N];

bool search(int cur){
  if( cur == N ){

    // cout << "test" << endl;

    bool corr[K] = {false,};

    for(int j = 0; j< K; ++j){
      if( sets[j] == 1 ) corr[j] = true;
      if( sets[j] == buf[0] * buf[1] * buf[2] ) corr[j] = true;
    }

    for(int i = 0; i < N; ++i){
      for(int j = 0; j < K; ++j){
        if( sets[j] == buf[i] ){
          corr[j] = true;
        }
      }
    }

    for(int i = 0; i < N; ++i){
      for(int j = i+1; j < N;++j){
        for(int k = 0; k < K; ++k){
          if( sets[k] == buf[i] * buf[j] ){
            corr[k] = true;
          }
        }
      }
    }

    for(int i = 0; i < K; ++i){
      if( corr[i] != true ) return false;
    }

    return true;
  }else{

    for(int i = 2; i <= M; ++i){
      buf[cur] = i;
      if ( search(cur+1) ) return true;
    }
    
  }
  return false;
}

int main()
{
  int T;
  cin >> T;
 
  int R = 100;
  cout << "Case #1:" << endl;
  int dam;
  cin >> dam >> dam >> dam >> dam;
  // N >> M >> K;
  for(int i = 0; i < R; ++i){
    for(int j = 0; j < K; ++j){
      cin >> sets[j];
    }
    if( search(0) ){
      for(int j = 0; j < N; ++j){
        cout << buf[j];
      }
      cout << endl;
    }else{
    }
  }
}
