#include<iostream>
#include<cmath>
#define MAX 10000000000000LL
using namespace std;

int main(void){
  int t,p,n,x,c[1024][12];
  long long ans[1024][12][15];
  
  cin>>t;
  for(int count=1;count<=t;++count){
    cin>>p;
    n=(int)ceil(pow(2.,p));
    for(int j=0,s=n;j<n;++j,s/=2)
      for(int i=0;i<s;++i)    
        for(int k=0;k<=p;++k)
          ans[i][j][k] = MAX;
    for(int i=0;i<n;++i){
      cin>>x;
      x = p-x;
      for(int j=x;j<=p;++j)
        ans[i][0][j] = 0;
    }
    for(int j=1,s=n/2;j<=p;++j,s/=2)
      for(int i=0;i<s;++i)
        cin>>c[i][j];
    for(int j=1,s=n/2;j<=p;++j,s/=2)
      for(int i=0;i<s;++i)
        for(int k=0;k<=p;++k){
          ans[i][j][k] <?= ans[i*2][j-1][k] + ans[i*2+1][j-1][k];
          if(k<p)
            ans[i][j][k] <?= c[i][j] + ans[i*2][j-1][k+1] + ans[i*2+1][j-1][k+1];
        }  
    cout<<"Case #"<<count<<": "<<ans[0][p][0]<<endl;
  }
  return 0;
}
