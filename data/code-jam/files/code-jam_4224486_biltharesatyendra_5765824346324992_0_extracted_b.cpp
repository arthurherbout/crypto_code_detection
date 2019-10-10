#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int a[100005];
int rem[9765655];
int ass[9765655];
int main()
{

  int t;
scanf("%d",&t);
int ca;
for(ca=1;ca<=t;ca++)
{
int b,n;
scanf("%d%d",&b,&n);

int i;
int hi = 1;
for(i=1;i<=b;i++){
scanf("%d",&a[i]);
hi*=a[i];
}
int total = 0;
for(i=1;i<=b;i++)
total+=hi/a[i];

n = n%total;
if(n==0)n=total;
//printf("checking for %d\n",n);
int k;
int ans;
for(i=1;i<=b;i++)rem[i] = 0;
for(k=1;k<=n;k++)
{
 ans = 1;
for(i=1;i<=b;i++)
{
if(rem[i]<rem[ans])ans = i;


}

rem[ans]+= a[ans];
ass[k] = ans;
//printf("assigning %d to %d and it becomes %d\n",k,ans,rem[ans]);
}
printf("Case #%d: %d\n",ca,ass[n]);


}

  return 0;
}
