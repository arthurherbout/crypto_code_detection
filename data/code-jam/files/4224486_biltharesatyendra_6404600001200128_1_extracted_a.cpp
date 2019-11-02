#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
int a[100005];
int main()
{

  int t;
scanf("%d",&t);
int ca;
for(ca=1;ca<=t;ca++)
{

int n;
scanf("%d",&n);
int i;
for(i=0;i<n;i++)
scanf("%d",&a[i]);
int fir,sec;
fir = sec = 0;
int hi = 0;
for(i=0;i<n-1;i++)
{
if(a[i] > a[i+1]){
int cur = a[i] - a[i+1];
fir+=cur;
if(cur>hi)hi = cur;
}
}
for(i=0;i<n-1;i++)
{
int tm;
if(a[i] > hi)sec+=hi;
else sec+=a[i];
}

printf("Case #%d: %d %d\n",ca,fir,sec);



}

  return 0;
}
