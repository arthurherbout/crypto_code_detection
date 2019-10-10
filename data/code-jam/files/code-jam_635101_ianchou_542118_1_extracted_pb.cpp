#include<iostream>
#include<algorithm>
using namespace std;
class chick{
  public:
    int x,v;
};
bool cmp(chick a, chick b){
  return a.x > b.x;
}
int main(void){
  int c,n,k,b,t,ans,done;
  chick data[100];
  cin>>c;
  for(int z=1;z<=c;++z){
    cin>>n>>k>>b>>t;
    for(int i=0;i<n;++i)
      cin>>data[i].x;
    for(int i=0;i<n;++i)
      cin>>data[i].v;
    sort(data, data+n, cmp);
    ans = 0;
    done = 0;
    for(int i=0;i<n && done<k;++i){
      if(b-data[i].x <= t*data[i].v)
        ++done;
      else
        ans += k - done;
    }
    cout<<"Case #"<<z<<": ";
    if(done < k)
      cout<<"IMPOSSIBLE"<<endl;
    else 
      cout<<ans<<endl;
  }
  return 0;
}
