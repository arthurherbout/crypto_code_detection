#include<iostream>
#include<string>
#include<sstream>
using namespace std;
int main(){
    int t,cas=0;
    cin>>t;
    while(t--){
        string s;
        cin>>s;
        string ans="";
        ans+=s[0];
        for(int i=1;i<s.length();i++){
            if(s[i]>=ans[0]){
                ans=s[i]+ans;
            }
            else{
               ans+=s[i];
            }
        }
        cout<<"Case #"<<++cas<<": "<<ans<<endl;
    }
}

