#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#include<string>
using namespace std;
class Bur{
    public:
    int id,time;
    Bur(int id_,int time_){
        id=id_;
        time=time_;
    }
};
class mycomparison
{
public:
  bool operator() (const Bur& a, const Bur&b) const
  {
      if(a.time==b.time){
        return a.id<b.id;
      }
      return a.time<b.time;
  }
};
int gcd(int x,int y){
    while(x%=y)swap(x,y);
    return y;
}
int main(){
    int t;
    cin>>t;
    for(int cas=1;cas<=t;cas++){
        vector<int> btime;
        priority_queue<Bur,vector<Bur>,mycomparison> qu;
        int b,n;
        cin>>b>>n;
        int tmp;
        cin>>tmp;
        btime.push_back(tmp);
        qu.push(Bur(0,tmp));
        int gcdmun=tmp;
        for(int i=1;i<b;i++){
            cin>>tmp;
            btime.push_back(tmp);
            qu.push(Bur(i,tmp));
            gcdmun=gcd(gcdmun,tmp);
        }
        //cout<<gcdmun<<endl;
        int round=0;
        for(int i=0;i<b;i++){
            round+=btime[i]/gcdmun;
        }
        int ans=n%round;
        if(ans==0)ans=round;
        for(int i=0;i<ans-1;i++){
            Bur bb=qu.top();
            qu.pop();
            qu.push(Bur(bb.id,bb.time+bb.time));
        }
        Bur nowa=qu.top();
        cout<<"Case #"<<cas<<": "<<nowa.id+1<<endl;
    }
}
