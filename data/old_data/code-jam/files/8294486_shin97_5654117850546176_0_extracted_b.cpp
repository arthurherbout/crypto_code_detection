
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
    vector<int> a(6),b(6),aa(6),bb(6);
    int n;cin>>n;
    for(int i=0;i<6;++i)cin>>a[i];
    for(int i=0;i<6;++i)a[i]*=2;
    for(int i=0;i<6;++i)aa[i]=a[i]+a[(i+1)%6];
    for(int i=0;i<6;++i){
      for(int j=2;j<5;++j)b[i]+=a[(i+j)%6];
      for(int j=2;j<6;++j)bb[i]+=a[(i+j)%6];
      // cout<<b[i]<<endl;
    }
    string res="";
    string me="ROYGBV";
    bool impossible =false;
    for(int i=0;i<6;++i)if(a[i]>b[i])impossible=1;
    for(int i=0;i<6;++i)if(aa[i]>bb[i])impossible=1;
    if(impossible){
      cout<<"Case #"<<(tt+1)<<": "<<"IMPOSSIBLE"<<endl;
      continue;
    }
    int k=0;
    while(a[k]==0)k++;
    for(int i=0;i<n;++i){
      a[k]--;
      for(int j=2;j<5;++j){
        b[(k+j)%6]--;
      }
      aa[k]--;
      aa[(k+5)%6]--;
      for(int j=1;j<5;++j){
        bb[(k+j)%6]--;
      }
      res=res+me[k];
      for(int j=k+2;j<k+5;++j){
        if(a[j%6]==0)continue;
        a[j%6]--;
        for(int l=2;l<5;++l){
          b[(j+l)%6]--;
        }
        aa[j%6]--;
        aa[(j+5)%6]--;
        for(int l=1;l<5;++l){
          bb[(j+l)%6]--;
        }
        bool ok = true;
        for(int l=0;l<6;++l)if(a[l]>b[l])ok=false;
        for(int l=0;l<6;++l)if(aa[l]>bb[l])ok=false;
        if(ok){
          k=j%6;
          break;
        }
        else{
          a[j%6]++;
          for(int l=2;l<5;++l){
            b[(j+l)%6]++;
          }
          aa[j%6]++;
          aa[(j+5)%6]++;
          for(int l=1;l<5;++l){
            bb[(j+l)%6]++;
          }
        }
      }
    }
    cout<<"Case #"<<(tt+1)<<": "<<res<<endl;
  }
  return 0;
}
