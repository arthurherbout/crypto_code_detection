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
    long long n,k;cin>>n>>k;
    long long d=1ll;
    k--;
    while(d-1<=k)d<<=1;
    d>>=1;
    k-=(d-1);
    n-=(d-1);
    long long a=n/d,b=n%d;
    long long res;
    if(k<b)res=a+1;
    else res=a;
    res--;
    cout<<(res/2 + res%2)<<" "<<(res/2)<<endl;
  }
  return 0;
}
