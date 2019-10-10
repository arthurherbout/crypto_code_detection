#include<iostream>
#include<cmath>
using namespace std;
int n;
int fib[100]={0,1,1};
int pow2[100]={1};
int ans[50];

int tt[2][2];
int NN[2][2];
int temp[2][2];

void muttn()
{
  for(int i=0;i<2;i++)
   for(int j=0;j<2;j++)
   {
     temp[i][j]=0;
     for(int k=0;k<2;k++)
     {
       temp[i][j]+=(tt[i][k]*NN[k][j])%1000;
     }
     temp[i][j]%=1000;
   }
  for(int i=0;i<2;i++)
    for(int j=0;j<2;j++)
    tt[i][j]=temp[i][j];
}

void mutnn()
{
  for(int i=0;i<2;i++)
   for(int j=0;j<2;j++)
   {
     temp[i][j]=0;
     for(int k=0;k<2;k++)
     {
       temp[i][j]+=(NN[i][k]*NN[k][j])%1000;
     }
     temp[i][j]%=1000;
   }
  for(int i=0;i<2;i++)
    for(int j=0;j<2;j++)
    NN[i][j]=temp[i][j];
}

int main()
{
  freopen("lin.txt","r",stdin);
  freopen("out.txt","w",stdout);
  int t;
  int line=0;
  scanf("%d",&t);
  while(t--)
  {
    scanf("%d",&n);
    int ans=0;
    int ans1=0;
    long long int b=2LL*n-1;
    tt[0][0]=1;
    tt[0][1]=0;
    tt[1][0]=0;
    tt[1][1]=1;
    NN[0][0]=0;
    NN[0][1]=1;
    NN[1][0]=1;
    NN[1][1]=1;    
    while(b)
    {
      if(b&1) muttn();
      mutnn();
      b>>=1;
    }    
    ans1=tt[0][1];
    b=n;
    int t1=1,n1=2;
    while(b)
    {
      if(b&1) t1=(t1*n1%1000)%1000; 
      n1=(n1*n1)%1000;
      b>>=1;
    }
    ans1=(t1*2*ans1)%1000;
    ans+=ans1;
    ans1=tt[0][0]+tt[0][1];
    ans+=(ans1*t1)%1000;
    ans-=1;
    ans%=1000;   
    printf("Case #%d: %03d\n",++line,ans);     
  }
  
}
