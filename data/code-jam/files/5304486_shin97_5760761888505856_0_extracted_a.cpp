
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
    int m,n;cin>>n>>m;
    set<char> se;
    map<char,int> l,r,u,d;
    vector<string> cake(n);
    for(int i=0;i<n;++i){
      cin>>cake[i];
      for(int j=0;j<m;++j){
        if(cake[i][j]!='?'){
          char c=cake[i][j];
          if(se.find(c)==se.end()){
            se.insert(c);
            l[c]=j;
            r[c]=j;
            d[c]=i;
            u[c]=i;
          }
          else{
            l[c]=min(l[c],j);
            r[c]=max(r[c],j);
            u[c]=min(u[c],i);
            d[c]=max(d[c],i);
          }
        }
      }
    }
    for(char c:se){
      for(int i=u[c];i<=d[c];i++){
        for(int j=l[c];j<=r[c];j++){
          cake[i][j]=c;
        }
      }
    }
    for(char c:se){
      int k=u[c];
      while(true){
        k--;
        if(k<0)break;
        bool ok=true;
        for(int i=l[c];i<=r[c];i++)if(cake[k][i]!='?')ok=false;
        if(!ok)break;
        for(int i=l[c];i<=r[c];i++)cake[k][i]=c;
      }
      u[c]=k+1;
      k=d[c];
      while(true){
        k++;
        if(k>n-1)break;
        bool ok=true;
        for(int i=l[c];i<=r[c];i++)if(cake[k][i]!='?')ok=false;
        if(!ok)break;
        for(int i=l[c];i<=r[c];i++)cake[k][i]=c;
      }
      d[c]=k-1;
    }
    // cout<<"1"<<endl;
    for(char c:se){
      int k=l[c];
      // cout<<2<<endl;
      while(true){
        k--;
        if(k<0)break;
        bool ok=true;
        for(int i=u[c];i<=d[c];i++)if(cake[i][k]!='?')ok=false;
        if(!ok)break;
        for(int i=u[c];i<=d[c];i++)cake[i][k]=c;
      }
      l[c]=k+1;
      k=r[c];
      while(true){
        k++;
        if(k>m-1)break;
        bool ok=true;
        for(int i=u[c];i<=d[c];i++)if(cake[i][k]!='?')ok=false;
        if(!ok)break;
        for(int i=u[c];i<=d[c];i++)cake[i][k]=c;
      }
      r[c]=k-1;
    }
    cout<<"Case #"<<(tt+1)<<":"<<endl;
    for(int i=0;i<n;++i)cout<<cake[i]<<endl;
  }
  return 0;
}
