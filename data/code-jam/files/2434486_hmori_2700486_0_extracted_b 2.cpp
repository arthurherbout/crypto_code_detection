#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;
typedef long long int LL;


vector<int> pyrSize;

double Comb(int N, int R){
  double ret = 1.;
  R = min(N-R,R);
  for(int r = 1; r <= R; r++){
    ret *= (double)(N-r+1)/(double)r;
  }
  return ret;
}

double solve(int N, int X, int Y){
  // First construct a pyramid shape
  // Then, fill the outer shell
  vector<int>::iterator it = (upper_bound(pyrSize.begin(), pyrSize.end(), N));
  int pyrHeight = it - pyrSize.begin();
  bool inPyr = ((abs(X)+abs(Y)) <= ((pyrHeight-1)*2));
  bool inShell = ((abs(X)+abs(Y)) == ((pyrHeight)*2));
  //cout << N << ' '<<  X << ' ' << Y << '\t';
  if(inPyr){

    //cout << "inPyr ";
    return 1.;

  }else if(inShell){
    //cout << "inShell ";
    int remTrials = N - pyrSize[pyrHeight-1];
    if(remTrials == 0){
      return 0;
    }
    int sureHeight = remTrials - ((pyrSize[pyrHeight]-pyrSize[pyrHeight-1]-1)/2); // there is if Y <= sureHeight
    int targetHeight = Y;
    if(targetHeight < sureHeight){
      //cout << "Sure Height " << sureHeight << ' ';
      return 1.;
    }else{
      if(X == 0){
	return 0.;
      }
      double sum = 0.;
      for(int h = targetHeight+1; h <= remTrials; h++){
	sum+=Comb(remTrials,h);
      }
      return sum / pow(2.,(double)remTrials);
    }
  }else{
    return 0.;
  }
}

int main(){

  for(int i = 1;;i+=2){
    int size = ((i+1)*(i))/2;
    if(size <= 10000000){
      pyrSize.push_back(size);
    }else{
      break;
    }    
  }

  int T;
  cin >> T;
  for(int t = 0; t < T; t++){
    int N, X, Y;
    cin >> N >> X >> Y;
    cout << "Case #" << t+1 << ": " <<  solve(N,X,Y) << endl;
  }
  return 0;
}
