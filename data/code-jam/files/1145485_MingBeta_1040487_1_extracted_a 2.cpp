#include<iostream>
#include<cstdio>
#include<fstream>
#include<algorithm>
#include<cmath>
#include<vector>
#include<map>
#include<cstring>

using namespace std;

long long n,pg,pd;
bool flag;

long long gcd(long long a,long long b)
{
     if (a%b==0) return b;
     return gcd(b,a%b);
}

int main()
{
    freopen("A-large.in","r",stdin);
    freopen("A-large.out","w",stdout);
    int cs;
    cin>>cs;
    for (int k=1;k<=cs;k++){
        cin>>n>>pd>>pg;
        flag=true;
        if (pg==0){
           if (pd) flag=false;
           } else 
        if (pg==100){
           if (pd!=100) flag=false;
           } else 
        if (pd){
           long long d=gcd(100,pd);
           long long c=100/d;
           if (n<c) flag=false;
           }
        cout<<"Case #"<<k<<": ";
        if (flag) cout<<"Possible"<<endl;
           else cout<<"Broken"<<endl;
        }
}
