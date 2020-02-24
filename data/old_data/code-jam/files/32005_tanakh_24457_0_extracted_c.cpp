#include <iostream>
#include <iomanip>
#include <sstream>
#include <complex>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
using namespace std;

double rec(int m,int x,double p,vector<vector<double> > &tbl)
{
  if (x>=1000000) return 1;
  if (m==0) return 0;

  x=x/(1000000/32)*(1000000/32);
  double &ret=tbl[m][x];

  if (ret>=-0.5) return ret;

  ret=0;

  for (int i=0;i<=x;i++){
    double tmp=
      (rec(m-1,x+i,p,tbl)*p+
       rec(m-1,x-i,p,tbl)*(1-p));
    //cout<<tmp<<" "<<i<<endl;
    ret=max(ret,tmp);
  }

  return ret;
}

void solve()
{
  int m,x;
  double p;
  cin>>m>>p>>x;
  vector<vector<double> > tbl(m+1,vector<double>(1000001,-1));
  cout<<rec(m,x,p,tbl)<<endl;
}

int main()
{
  cout<<setiosflags(ios::fixed)<<setprecision(10);
  int cases;
  cin>>cases;
  for (int c=1;c<=cases;c++){
    cout<<"Case #"<<c<<": ";
    solve();
  }
  return 0;
}
