#include<stdio.h>
#include<stdio.h>
#include<string>
#include<string.h>
#include<algorithm>
#include<math.h>
using namespace std;
int is(int m)
{
    char a[10],b[10];
    sprintf(a,"%d",m); sprintf(b,"%d",m); reverse(b,b+strlen(b));
    string c,d;c=a; d=b;
    return c==d;
}
int fair(int m)
{
    if(!is(m)) return 0;
    int x=(int) sqrt(m);
    if(x*x !=m)return 0;
    if(!is(x))return 0;
    return 1;
}
int main()
{
    int m;
   int a[1005];
   for(m=1;m<=1001;m++)a[m] = fair(m);
   int t,ca,n;
   scanf("%d",&t);
   for(ca=1;ca<=t;ca++)
   {
                       scanf("%d%d",&m,&n);
                       int i,count;
                       count=0;
                       for(i=m;i<=n;i++)count=count+a[i];
                       printf("Case #%d: %d\n",ca,count);
                       }
    
  
}
