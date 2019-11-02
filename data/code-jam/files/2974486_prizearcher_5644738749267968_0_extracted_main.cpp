#include <cstdlib>
#include <cstdio>
#include <iostream>

using namespace std;

FILE *f1=fopen("D-small-attempt0.in","r");
FILE *f2=fopen("D-small-attempt0.out","w");

int t,n,ans1,ans2,x,l;
double a[1000],b[1000];

int cmp( const void *a , const void *b )
{
    return (*(double *)a)<(*(double *)b)?1:-1;
}

int main(int argc, char *argv[])
{
    fscanf(f1,"%d",&t);
    for(int k=1;k<=t;k++)
    {
      fscanf(f1,"%d",&n);ans1=ans2=x=l=0;
      for(int i=0;i<n;i++)fscanf(f1,"%lf",&a[i]);
      qsort(a,n,sizeof(a[0]),cmp);
      for(int i=0;i<n;i++)fscanf(f1,"%lf",&b[i]);
      qsort(b,n,sizeof(b[0]),cmp);
      for(int i=0;i<n && x<n-l;i++)
      {
        if(b[i]>a[x])l++;
        else
        {
          x++;
          ans1++;
        }
      }
      x=0;
      for(int i=0;i<n;i++)
        if(b[x]>a[i])
        {
          x++;
          ans2++;
        }
      ans2=n-ans2;
      fprintf(f2,"Case #%d: %d %d\n",k,ans1,ans2);
    }
    return 0;
}
