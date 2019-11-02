#include<iostream>
#define MAX 100
using namespace std;
int main(void){
  int t,r,q[105][105],ans,x1,x2,y1,y2;
  bool end;
  cin>>t;
  for(int count=1;count<=t;++count){
    cin>>r;
    for(int i=0;i<MAX+5;++i)
      for(int j=0;j<MAX+5;++j)
        q[i][j]=0;
    ans=0;
    for(int i=0;i<r;++i){
      cin>>x1>>y1>>x2>>y2;
      for(int i=x1;i<=x2;++i)
        for(int j=y1;j<=y2;++j)
          q[i][j]=1;
    }
    while(1){
      end = 1;
      for(int i=MAX;i>=1;--i)
        for(int j=MAX;j>=1;--j)
          if(q[i][j]==1){
            end=0;
            if(q[i-1][j]==0 && q[i][j-1]==0)
              q[i][j]=0;
          }
          else{
            if(q[i-1][j]==1 && q[i][j-1]==1)
              q[i][j]=1;
          }
      if(end)
        break;
      else 
        ++ans;
    }
    cout<<"Case #"<<count<<": "<<ans<<endl;
  }
}
