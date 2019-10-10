
#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
using namespace std;


#define INF (1ll<<60)


int main(int argc, char const *argv[]) {
  int t;cin>>t;
  for(int tt=0;tt<t;tt++){
    double d;cin>>d;
    int n;cin>>n;
    double r=0.0;
    for(int i=0;i<n;++i){
      double dd,s;cin>>dd>>s;
      r=max(r,(d-dd)/s);
    }
    double res=d/r;

    cout<<"Case #"<<(tt+1)<<": "<<setprecision(50)<<res<<endl;
    // for(int i=0;i<n;++i)cout<<cake[i]<<endl;
  }
  return 0;
}
