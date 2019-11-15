#include<iostream>
#include<cstdio>
#include<climits>
#include<string>
#include<cstring>
#include<algorithm>
#include<vector>
#include<stack>
#include<queue>
#include<set>
#include<cmath>
#include<queue>
using namespace std;
#define inp(a) scanf("%lld",&a)
#define line(a) printf("%lld ",a);
#define next() printf("\n");
#define out(a) printf("%lld\n",a)
#define swap(a,b) {a=a+b;a=a-b;b=a-b;}
#define VI vector<int>
#define VLL vector<long long int>
#define PQI priority_queue<int>
#define PQLL priority_queue<long long int>
#define ll long long int
#define mod 1000000007
#define getcx getchar_unlocked
/*inline void fscan(ll *a )
{
ll n=0; int ch = getcx(); int sign = 1;
while(ch < '0' || ch > '9')
{
if(ch == '-') sign=-1; ch = getcx();
}
while(ch >= '0' && ch <= '9')
{
n = (n << 3) + (n << 1) + ch - '0', ch = getcx();
}
*a = n * sign;
}*/
int i,j,k;
double a[10000],b[10000];
int main()
{
int t,l;
scanf("%d",&t);
for(l=1;l<=t;l++)
{
int n;
scanf("%d",&n);
for(i=0;i<n;i++)
scanf("%lf",&a[i]);
for(i=0;i<n;i++)
scanf("%lf",&b[i]);
sort(a,a+n);
sort(b,b+n);

int ai=0,bi=0,ae=0,be=0,ga1=0,ga2=0,gb1=0,gb2=0;
k=0;
for(i=0;i<n;i++)
{
for(j=k;j<n;j++)
{
if(b[j]>a[i])
{
gb1++;
j++;
break;
}
}
k=j;
}
int ans2=n-gb1,del=0,ini=0;
k=n-1;
for(i=n-1;i>=ini;i--)
{
for(j=k;j>=0;j--)
{
if(b[j]>a[i])
{
ini++;
del++;
}
else
{
j--;
break;
}
}
k=j;

}
printf("Case #%d: %d %d\n",l,n-ini,ans2);	
}
return 0;
}
