#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include<iomanip>
#include<map>

using namespace std;

int main()
{
  int kk=0;
  int T;
  cin >> T;
  map<int,int> log2;
  for(int i=0;(1<<i)<10000000;i++){
    log2[1<<i]=kk;
    ++kk;
    //cout << i << ' ' << log2[i<<i] << endl;
  }
  
  for(int tc=1;tc<=T;++tc){
    int C;
    double D;
    cin >> C >> D;
    if(D<0)D=-D;
    map<int,int> pos;
    for(int i = 0; i < C; ++i){
      int P;
      int V;
      cin >> P >> V;
      pos[P]+=V;
    }

    double res = 0;    
    for(map<int,int>::iterator it = pos.begin(); it != pos.end(); ++it){
      double p1 = (double)it->first;
      ++it;
      if(it==pos.end()){
        break;
      }
      double p2 = (double)it->first;
      if( abs(p1-p2)<D ){
        res += abs(abs(p1-p2)-D)/2;
      }
    }
    
    for(map<int,int>::iterator it = pos.begin(); it != pos.end(); ++it){
      int num = it->second;
      if(num>=2){
        if(num%2==0){
          res += D/2;
          if(num>2){
            res += D*((num-2)/2);
          }
        }else{
          res += D*(num/2);
        }
      }
    }

    cout <<"Case #" << tc << ": ";
    cout << fixed<<setprecision(11)<<res << endl;
  }
  return 0;
}
