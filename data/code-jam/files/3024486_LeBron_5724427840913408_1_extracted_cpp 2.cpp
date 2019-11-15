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

long tests,p,q,n,h[2000],g[2000],bst[400][4000];
long rem,need,np;
long answ;
long ts;

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
 cin>>p>>q>>n;
 for (int i=1;i<=n;i++)
 {
  cin>>h[i]>>g[i];
 }
 for (int i=0;i<=n;i++)
  for (int r=0;r<=2000;r++)
  {
   bst[i][r]=-1e9;
  }
 bst[0][1]=0;
 for (int i=0;i<=n;i++)
   for (int j=0;j<=1000;j++)
    if (bst[i][j]>=0)
    {
     // don't attack
     bst[i+1][j+h[i+1]/q+(h[i+1]%q>0)]=max(bst[i+1][j+h[i+1]/q+(h[i+1]%q>0)],bst[i][j]);
     // mvs twr
     for (int twr=0;twr<=10;twr++)
     {
      rem=h[i+1]-twr*q;
      if (rem<=0)continue;
      need=rem/p+(rem%p>0);
      np=j-need+twr;
      if (np>=0)bst[i+1][np]=max(bst[i+1][np],bst[i][j]+g[i+1]);
     }
    }
 answ=0;
 for (int i=0;i<=1000;i++)
  answ=max(answ,bst[n][i]);
  cout<<"Case #"<<ts<<": ";
 cout<<answ<<endl;
}

cin.get();cin.get();
return 0;}
