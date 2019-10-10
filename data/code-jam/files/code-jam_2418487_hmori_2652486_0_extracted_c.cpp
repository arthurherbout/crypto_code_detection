#include <iostream>
#include <string>
#include <cmath>
using namespace std;

int num[7]; // 7 for small, 12 for large

void solve(int K){
  int count2 =0;
  int count3=0;
  int count4=0;
  int count5=0;
  for(int k = 0; k < K; k++){
#define CHKMOD(field) if(num[k] % field == 0)
    CHKMOD(125)
      count5 = 3;      
    CHKMOD(25)
      count5 = max(2,count5);      
    CHKMOD(5)
      count5 = max(1,count5);      
    CHKMOD(64)
      count4 = 3;      
    CHKMOD(16)
      count4 = max(2,count4);      
    CHKMOD(27)
      count3 = max(3,count3);      
    CHKMOD(9)
      count3 = max(2,count3);      
    CHKMOD(3)
      count3 = max(1,count3);
    CHKMOD(36)
      {
	count4 = 1;
	count3 = 2;
      }
    CHKMOD(48)
      {
	count4 = 2;
	count3 = 1;
      }
    CHKMOD(80)
      {
	count5 = 1;
	count4 = 2;
      }
    CHKMOD(100)
      {
	count5 = 2;
	count4 = 1;
      }
    CHKMOD(60)
      {
	count3 = 1;
	count4 = 1;
	count5 = 1;
      }
  }
  for(int k = 0; k < K; k++){
    if(num[k] % 4 == 0 && count3+count5 == 2){
      count4 = 1;
    }
  }

  int restCount = 3 - count5 - count4- count3;
  count2 = restCount;
  
  for(int i = 0; i < count2; i++){
    cout << '2';
  }
  for(int i = 0; i < count3; i++){
    cout << '3';
  }
  for(int i = 0; i < count4; i++){
    cout << '4';
  }
  for(int i = 0; i < count5; i++){
    cout << '5';
  }

  cout << endl;
}


int main(){
  int T, R, N, M, K;
  cin >> T >> R >> N >> M >> K;
  cout << "Case #1:" << endl;
  for(int r = 0; r < R; r++){
    for(int k = 0; k < K; k++){
      cin >> num[k];
    }
    solve(K);
  }
}
