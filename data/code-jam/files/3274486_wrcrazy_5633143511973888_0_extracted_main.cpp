#include <iostream>
#include <string.h>
#include<algorithm>
#include<stdio.h>
#include<math.h>
using namespace std;
double pai = acos(-1);
double p[1011];
int main() {
  int t,n,k;
  freopen("a.txt", "r", stdin);
  freopen("output.txt", "w", stdout);
  cin >> t;
  for (int ii = 1; ii <= t; ++ii) {

    cin >> n>>k;
    double kn;
    scanf("%lf",&kn);
    kn*=10000;
    for(int i=0;i<n;i++)
    {
        scanf("%lf",&p[i]);
        p[i]*=10000;
    }
    n++;
    p[n-1] = 10000;
    //cout<<n<<" "<<k<<" "<<kn<<endl;
    if(n>1)
        while(kn>0)
        {
            sort(p,p+n);
            //cout<<p[0]<<" "<<p[1]<<" "<<kn<<endl;
            //system("pause");
            if(p[0] >= 9999.9999)break;
            int j=0;
            while(p[j] == p[0])
                j++;
            double dif = (p[j]-p[0])*(j);
            if(kn < dif)
                dif = kn;
            kn -= dif;
            for(int zz=0;zz<j;zz++)
                p[zz] += dif/(j);
        }
    else
    {
        p[0]+=kn;
        if(p[0]>1)
            p[0]=1.0;
    }
    double ans =1.0;
    for(int i=0;i<n;i++)
    {
        ans*=p[i]/10000;
    }
    printf("Case #%d: %.10lf\n",ii,ans);
  }
}
