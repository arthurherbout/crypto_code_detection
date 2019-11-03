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
    cout<<"Case #"<<(tt+1)<<": ";
    long long n;cin>>n;
    long long t = 1;
    while(t<n){
      if((n/t)%10ll<(n/t/10ll)%10){
        n-=n%(10ll*t);
        n--;
      }
      t*=10;
    }
    cout<<n<<endl;
  }
  return 0;
}
