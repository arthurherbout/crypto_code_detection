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

long tests,p,q,n,h[2000],g[2000];
vector<long> v;
long ts;
set<long long> was;
map<long long, long long> ans;
long long md;

long solve(vector<long > v)
{
long long temp=0;
 for (int i=0;i<v.size();i++)
  temp=temp*1373+v[i];
 if (was.find(temp)!=was.end())return ans[temp];
 
 long er=0;
 for (int i=0;i<v.size();i++)
  if (v[i])er=1;
 if (er==0)return 0;
 
 long tc;
 vector<long> v1;
 long bst=0;
 
 for (int i=0;i<v.size();i++)
 {
  if (v[i]>0){tc=0;v1=v;v1[i]-=p;if (v1[i]<=0){tc+=g[i+1];
  v1[i]=0;}q=0;while (q<v1.size()&&v1[q]==0)++q;if (q<v1.size()){v1[q]-=md;if (v1[q]<0)v1[q]=0;}
  bst=max(bst,solve(v1)+tc);
  } 
 }
 v1=v;
  for (int i=0;i<1;i++)
 {
  tc=0;q=0;while (q<v1.size()&&v1[q]==0)++q;if (q<v1.size()){v1[q]-=md;if (v1[q]<0)v1[q]=0;}
  bst=max(bst,solve(v1)+tc);
 }
 was.insert(temp);
 ans[temp]=bst;
 return bst;
}

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
    was.clear();
    v.clear();
 cin>>p>>md>>n;
 for (int i=1;i<=n;i++)
  cin>>h[i]>>g[i];
 ++ts;
 cout<<"Case #"<<ts<<": ";
 for (int i=1;i<=n;i++)
  v.push_back(h[i]);
 cout<<solve(v)<<endl;
}

cin.get();cin.get();
return 0;}
