#include<iostream>
#include<cmath>
using namespace std;
int n;
int fib[100]={0,1,1};
int pow2[100]={1};
int ans[50];

int main()
{
  freopen("in.txt","r",stdin);
  freopen("out.txt","w",stdout);
  int t;
  int line=0;
  scanf("%d",&t);
  for(int i=3;i<100;i++)
    fib[i]=(fib[i-1]+fib[i-2])%1000;
  for(int i=1;i<100;i++)
  {
     pow2[i]=(pow2[i-1]*2)%1000;
  }
  for(int i=2;i<50;i++)
  {
    ans[i]=(pow2[i+1]*fib[2*i-1]+pow2[i]*fib[2*i]-1)%1000;
  }
  while(t--)
  {
    scanf("%d",&n);
    printf("Case #%d: %03d\n",++line,ans[n]);     
  }
  
}
