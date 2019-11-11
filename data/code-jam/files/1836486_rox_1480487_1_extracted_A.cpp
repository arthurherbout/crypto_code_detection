#include <cstdio>
#include <cmath>
using namespace std;
const double eps=1e-8;
int a[222];
int main(void)
{
  int t;
  scanf("%d",&t);
  for(int T=1;T<=t;T++){
    printf("Case #%d: ",T);
    int n; scanf("%d",&n);
    int x=0;
    for(int i=0;i<n;i++){
      scanf("%d",a+i);
      x+=a[i];
    }
    double l,r,m;
    l=0; r=x;
    for(int iii=0;iii<50;iii++){
      m=(l+r)/2;
      double s=0;
      for(int i=0;i<n;i++){
        if(m-a[i]>0) s+=(m-a[i]);
      }
      //printf("%f %f\n",m,s);
      if(s<=x+eps) l=m;
      else r=m;
    }
    //printf("%f\n",r);
    for(int i=0;i<n;i++){
      double aa=(m-a[i]+eps)/x*100;
      aa=aa<0?0:aa;
      printf("%.6f%c",aa,i==n-1?'\n':' ');
    }
  }
  return 0;
}
