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
    string s;cin>>s;
    long long n;
    n = s.length();
    int k;cin>>k;
    int res=0;
    vector<bool> mem(n);
    for(int i=0;i<n;++i){
      mem[i]=(s[i]=='+');
    }
    for(int i=0;i<=n-k;++i){
      if(!mem[i]){
        for(int j=0;j<k;++j){
          mem[i+j]=mem[i+j]^true;
        }
        res++;
      }
    }
    bool ok=true;
    for(int i=0;i<k;i++)if(!mem[n-1-i])ok=false;
    if(ok)cout<<res<<endl;
    else cout<<"IMPOSSIBLE"<<endl;
  }
  return 0;
}
