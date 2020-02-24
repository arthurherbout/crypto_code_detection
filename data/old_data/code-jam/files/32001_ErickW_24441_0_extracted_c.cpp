#include <cmath>
#include <map>
#include <set>
#include <algorithm>
#include <iostream>

using namespace std; typedef unsigned long ulong; typedef long long llong;

int n; double m;

int feas(double m,int x[],int y[],int z[],int p[])
{
   double m000=-1e12,M000=1e12,m010=-1e12,M010=1e12,m001=-1e12,M001=1e12,m011=-1e12,M011=1e12;

   for(int i=0;i<n;i++)
   {
     double r=m*p[i];

     M000 = min(M000, (r+x[i]+y[i]+z[i]));
     m000 = max(m000, (-r+x[i]+y[i]+z[i]));
     M001 = min(M001, (r+x[i]+y[i]-z[i]));
     m001 = max(m001, (-r+x[i]+y[i]-z[i]));
     M011 = min(M011, (r+x[i]-y[i]-z[i]));
     m011 = max(m011, (-r+x[i]-y[i]-z[i]));
     M010 = min(M010, (r+x[i]-y[i]+z[i]));
     m010 = max(m010, (-r+x[i]-y[i]+z[i]));
   }

   if (m000 >= M000) return false;
   if (m001 >= M001) return false;
   if (m011 >= M011) return false;
   if (m010 >= M010) return false;

   return true;
}

int main()
{
int cases;

cin >> cases;

for(int loop=1;loop<=cases;loop++)
{
  int i,j,k;

  cin >> n;
  int x[n],y[n],z[n],p[n];

  for(i=0;i<n;i++) { cin >> x[i] >> y[i] >> z[i] >> p[i]; }

  double a=0,b=1e20;
  while(abs(b-a)>1e-7) { m=(b+a)*0.5; if (feas(m,x,y,z,p)) b=m; else a=m; }

  printf("Case #%d: %0.6lf\n",loop,m);
}

}
