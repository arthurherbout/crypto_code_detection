#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <cstring>
#include <sstream>
using namespace std;
#define p(x) cout<<#x<<":"<<x<<"\n"
#define mod 10009
#define lim 101
#define lim2 257

int cs,c,k,n,i,j,p,sz,x,y,s,res;
char str[lim],str2[lim],A[lim][lim];
int M[lim],S[lim],F[lim][lim2],C[lim2],fac[lim];

void rec(int i)
{
  int j,p,x,y;

  if(i)
    for(j=0;j<n;j++)
	{
	  for(p='a';p<='z';p++)
	    C[p]+=F[j][p];
	  rec(i-1);
	  for(p='a';p<='z';p++)
	    C[p]-=F[j][p];
	}
  else
  {
    for(j=x=0;j<sz;j++)
    {
	  for(p=0,y=1;p<S[j];p++)
	    y=(y*C[A[j][p]])%mod;
	  x=(x+y)%mod;
    }
    res=(res+x)%mod;
  }
}
int main()
{
  fac[0]=1;
  for(i=1;i<lim;i++)
    fac[i]=(fac[i-1]*i)%mod;
  scanf("%d",&cs);
  for(c=1;c<=cs;c++)
  {
    scanf("%s%d%d",str,&k,&n);
	memset(F,0,sizeof F);
	for(i=0;i<n;i++)
	{
	  scanf("%s",str2);
	  for(j=0,sz=strlen(str2);j<sz;j++)
	    F[i][str2[j]]++;
	}
	sz=strlen(str);
	for(i=0;i<sz;i++)
	  if(str[i]=='+')
	    str[i]=' ';
	stringstream ss(str);
	sz=0;
	while(ss>>A[sz])
	{
	  S[sz]=strlen(A[sz]);
	  sz++;
	}
/*	for(i=1;i<1<<n;i++)
	{
	  for(j=s=0;j<n;j++)
	    if(i&1<<j)
		  s++;
	  if(s==k)
	  {
	    memset(C,0,sizeof C);
	    for(j=0;j<n;j++)
		  if(i&1<<j)
		    for(p='a';p<='z';p++)
			  C[p]+=F[j][p];
		p(i);
		for(j='a';j<='z';j++)
		  if(C[j])
		    printf("%c %d\n",j,C[j]);
	    for(j=x=0;j<sz;j++)
		{
		  for(p=0,y=1;p<S[j];p++)
		    y=(y*C[A[j][p]])%mod;
		  x=(x+y)%mod;
//		  p(y);
		}
		p(x);
		M[s]=(M[s]+x*fac[s])%mod;
	  }
	}*/
	printf("Case #%d:",c);
	for(i=1;i<=k;i++)
	{
	  res=0;
	  memset(C,0,sizeof C);
	  rec(i);
	  printf(" %d",res);
	}
	printf("\n");
  }
  return 0;
}
