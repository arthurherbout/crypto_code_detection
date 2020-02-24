#include<iostream>
using namespace std;
int main(void){
  int t,n,ans[505];
  ans[2] = 1;
  ans[3] = 2;
  for(int i=4;i<505;++i)
    ans[i] = (ans[i-1] + ans[i-2]) % 100003;
  cin>>t;
  for(int z=1;z<=t;++z){
    cin>>n;
    cout<<"Case #"<<z<<": "<<ans[n]<<endl;
  }
}
