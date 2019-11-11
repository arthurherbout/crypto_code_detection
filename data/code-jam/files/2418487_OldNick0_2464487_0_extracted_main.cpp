#include<iostream>
#include<cmath>
using namespace std;
int main(){
    int t;
    cin>>t;
    for(int cas=1;cas<=t;cas++){
        long long r,t;
        cin>>r>>t;
        long long str=sqrt(t);
        str/=2;
        str-=(2*r-1);
        if(str<0){
            str=0;
        }
        for(;str*(2*str-1+2*r)<t;str++){
        }
        if(str*(2*str-1+2*r)<=t)cout<<"Case #"<<cas<<": "<<str<<endl;
        else cout<<"Case #"<<cas<<": "<<str-1<<endl;
    }
}
