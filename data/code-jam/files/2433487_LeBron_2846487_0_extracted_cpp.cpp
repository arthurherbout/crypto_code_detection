/*

*/

//#pragma comment(linker, "/STACK:16777216")
#include <fstream>
#include <iostream>
#include <string>
#include <math.h>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include <stdio.h>
#include <stack>
#include <algorithm>
#include <list>
#include <memory.h>
 
#define y0 sdkfaslhagaklsldk
#define y1 aasdfasdfasdf
#define yn askfhwqriuperikldjk
#define j1 assdgsdgasghsf
#define tm sdfjahlfasfh
#define lr asgasgash
 
#define  INF 100000000
#define eps 1e-9
//#define M_PI 3.141592653589793
//#define mx 1000000000000ll
#define bs 1000000007
//#define free asdfasdfsdadsg
//#define szz 400
//#define pb push_back
#define MAXN 10000
#define free afdshjioey
//#define SIZE 60
#define bsize 3

using namespace std;

long ts,mask,tests;
string st;
double ev[2000000],entev[2000000];
long n;
long arp[2000000];

int main(){
//freopen("palindrome.in","r",stdin);
//freopen("palindrome.out","w",stdout);
freopen("C:/input3.txt","r",stdin);
freopen("C:/output3.txt","w",stdout);
ios_base::sync_with_stdio(0);

cin>>tests;
for (;tests;--tests)
{
     ++ts;
     
     cin>>st;
     mask=0;
     
     n=st.size();
     
     for (int i=st.size()-1;i+1;i--)
     {mask*=2;if (st[i]=='X'){++mask;}
      }
     
     for (int i=0;i<(1<<n);i++)
     {
      ev[i]=0;
      entev[i]=0;
      arp[i]=0;
     }
  
     //-----------------------------
     for (int mmask=(1<<n)-2;mmask>=mask;--mmask)
     {
       //  cout<<mmask<<endl;
     long temp[25];
     
     for (int i=0;i<n;i++)
     {
         if (mmask&(1<<i))temp[i]=1;
         else temp[i]=0;
     }
     
     vector<long> vv;
     
     for (int i=0;i<n;i++)
     {
      long t=i;
      long evv=n;
      while (temp[t]==1){++t;if (t==n)t=0;--evv;}
      long nmask=(mmask|(1<<t));
      
   //   if (mmask==mask)cout<<nmask<<" "<<ev[nmask]<<" "<<evv<<" "<<ev[mmask]<<endl;
      ev[mmask]+=ev[nmask]*1.0/n+evv*1.0/n;
  //   if (mmask==mask)cout<<nmask<<" "<<ev[nmask]<<" "<<evv<<" "<<ev[mmask]<<endl;
      
      }
     }
     cout.precision(11);
//     cout<<fixed<<ev[mask]<<endl;
     cout<<fixed<<"Case #"<<ts<<": "<<ev[mask]<<endl;
     
}

cin.get();cin.get();
return 0;}
