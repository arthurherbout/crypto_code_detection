#include<iostream>
#include<string>
#include <cmath>
#include <algorithm>
using namespace std;

int main()
{
 int T,p,k,l,freq[100],flag,t,keyp;
 float a,b,c;
  string str;
cin>>T;
for(int i=0;i<T;i++)
{ str="";
  keyp=0;
  flag=0;
  cin>>p>>k>>l;
 for(int j=0;j<l;j++)
   cin>>freq[j];
sort(freq,freq+l);
if(p*k<l)
 {str="Impossible";
  flag=1;
   cout<<"Case #"<<i+1<<": "<<str<<endl;
   continue;
 }
 t=1;
 for(int j=0;j<l;j++)
   {  if(j!=0 && j%k==0)
        t++;
        keyp+= freq[l-j-1]*t;
   }
    
cout<<"Case #"<<i+1<<": "<<keyp<<endl;

 }
}
