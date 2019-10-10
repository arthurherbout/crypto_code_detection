#include <vector>
#include <iostream>
#include <cassert>
#include <string>
#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <map>
#include <set>
#include<sstream>

#define GETI ({int p;cin>>p;p;})
#define REP(i,st,end) for(typeof(st) i(st);i!=(end);i++)

using namespace std;
bool ugly(long long n){
     return (n%2==0 || n%3==0 || n%5==0 || n%7==0 || n==0) ;
     }
long long s2i(string s) {
	long long t;
	stringstream ss(s);
	ss>>t;
	return t;
}


int cnt=0;
void calc(long long traced_sum,string s,int st,int end){
     if(st>end){
                if(ugly(traced_sum))cnt++;
                //cout<<"tracer-"<<traced_sum<<"\n";
                return;
                }
     for(int i=st;i<=end;i++){
             long long tmp=s2i(s.substr(st,i-st+1));
             calc(traced_sum+tmp,s,i+1,end);
             calc(traced_sum-tmp,s,i+1,end);
             }
     return;
}
void process(int t){
     string n;
     
     cnt=0;
     cin>>n;
     for(int i=0;i<n.size();i++){
             long long tmp=s2i(n.substr(0,i+1));
             calc(tmp,n,i+1,n.size()-1);
     }
     cout<<"Case #"<<t<<": "<<cnt<<"\n";
     
}
int main(){
    
    int t;
    cin>>t;
    for(int i=0;i<t;i++)
    process(i+1);
    
    return 0;
}
