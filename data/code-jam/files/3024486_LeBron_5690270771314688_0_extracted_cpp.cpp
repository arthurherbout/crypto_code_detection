/*
*/

#pragma comment(linker, "/STACK:16777216")
#include <fstream>
#include <iostream>
#include <string>
#include <complex>
#include <math.h>
#include <set>
#include <vector>
#include <map>
#include <queue>
#include <stdio.h>
#include <stack>
#include <algorithm>
#include <list>
#include <ctime>
#include <memory.h>

#define y0 sdkfaslhagaklsldk
#define y1 aasdfasdfasdf
#define yn askfhwqriuperikldjk
#define j1 assdgsdgasghsf
#define tm sdfjahlfasfh
#define lr asgasgash
 
#define eps 1e-11
//#define M_PI 3.141592653589793
#define bs 1000000007
#define bsize 256

using namespace std;

long tests,ts,dp[20][(1<<18)];
string st;
long q;
long bst;
long n,tmask;

long calc(long x)
{
 long r=0;
 for (int i=0;i<=15;i++)
  if (x&(1<<i))++r;
  return r;
}

set<long> ids;
set<long>::iterator it;
long cc; string z[20000];
long id[2000];
map<long,long> mapp;

int main(){
//freopen("trade.in","r",stdin);
//freopen("trade.out","w",stdout);
freopen("C:/input.txt","r",stdin);
freopen("C:/output.txt","w",stdout);
ios_base::sync_with_stdio(0);
//cin.tie(0);

cin>>tests;
for (;tests;--tests)
{
 ++ts;
 cin>>n;
 for (int i=0;i<=n;i++)
  for (int mask=0;mask<(1<<n);mask++)
   dp[i][mask]=0;
 
 for (int i=0;i<(1<<n);i++)
  dp[0][i]=1;
 
 ids.clear();
 ids.insert(0);
 cc=0;
 for (int i=1;i<=n;i++)
 {
  cin>>z[i]>>id[i];
  ids.insert(id[i]); 
 }
 for (it=ids.begin();it!=ids.end();it++)
  {mapp[(*it)]=cc;++cc;//cout<<(*it)<<"  "<<cc<<endl;
  }
  
 for (int i=1;i<=n;i++)
 {
 // cin>>st>>q;
 st=z[i];
 q=mapp[id[i]];
// cout<<st<<"   "<<q<<endl;
  for (int mask=0;mask<(1<<n);mask++)
  for (int ps=0;ps<n;ps++)
  {
   if (dp[i-1][mask]==0)continue;
   if (q>0&&q!=ps+1)continue;
   //cout<<i<<" %"<<ps<<endl;
   if (st=="L"&&(mask&(1<<ps))){tmask=(mask^(1<<ps));
   dp[i][tmask]=1;}
//   if (i==n)cout<<mask<<" "<<tmask<<endl;
  if (st=="E"&&((mask&(1<<ps))==0)){ tmask=(mask^(1<<ps));
   dp[i][tmask]=1;}
  }
 // cout<<dp[i][0]<<" "<<dp[i][1]<<endl;
 }
 cout<<"Case #"<<ts<<": ";
 bst=1e6;
 for (int i=0;i<(1<<n);i++)
  if (dp[n][i]){//cout<<"^"<<i<<endl;
  bst=min(bst,calc(i));}
 if (bst>1000)cout<<"CRIME TIME";
 else cout<<bst;
 cout<<endl;
  
}

cin.get();cin.get();
return 0;}
