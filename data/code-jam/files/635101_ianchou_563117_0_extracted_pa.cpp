#include<map>
#include<iostream>
#include<string>
using namespace std;
map<string, int> path;
int main(void){
  int t,n,m,ans;
  string q,s;
  cin>>t;
  for(int z=1;z<=t;++z){
    ans = 0;
    path.clear();
    cin>>n>>m;
    for(int i=0;i<n;++i){
      cin>>q;    
      for(int i=1;i<=q.size();++i){
        if(q[i]=='/' || i==q.size())
          path[q.substr(0,i)] = 1;
      }
    }
    for(int i=0;i<m;++i){
      cin>>q;
      for(int i=1;i<=q.size();++i){
        if(q[i]=='/' || i==q.size()){
          s = q.substr(0,i);
          if(path.find(s) == path.end()){
            ++ans;
            path[s] = 1;
          }
        }
      }
    }
    cout<<"Case #"<<z<<": " <<ans<<endl;
  }
  return 0;
}
