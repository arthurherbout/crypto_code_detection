#include <iostream>
#include <cstdio>
#include <cstring>

using namespace std;

long in[5005]={0};
long out[5005]={0};

char str[5005]={0};

long a[505][505]={0};
long b[505][505]={0};
long mark[505]={0};

long hash[505]={0};

long ans=0;
long flag=0;
long ss=0;

void ff(long x)
{
 long i;
 
 if(out[x]-in[x]>0)
  {
   ss+=out[x]-in[x];
   flag=1;
  }
 
 hash[x]=1;
 for(i=1;i<=34;i++)
   if(a[x][i]&&!hash[i])
    ff(i);
}

void work(long x)
{
 long k,i,j;
 long s=0;
 long tt;
 
 printf("Case #%ld: ",x);
 
 ans=0;
 ss=0;
 memset(in,0,sizeof(in));
 memset(out,0,sizeof(out));
 memset(a,0,sizeof(a));
 memset(hash,0,sizeof(hash));
 memset(str,0,sizeof(str));
 memset(b,0,sizeof(b));
 
 scanf("%ld",&k);
 scanf("%s",str+1);
 
 for(i=1;str[i+1];i++)
   {
    if(!b[str[i]-'a'+1][str[i+1]-'a'+1])
     {
      out[str[i]-'a'+1]++;
      in[str[i+1]-'a'+1]++;
      a[str[i]-'a'+1][str[i+1]-'a'+1]=a[str[i+1]-'a'+1][str[i]-'a'+1]=1;
      ans++;
      b[str[i]-'a'+1][str[i+1]-'a'+1]=1;
     }
    if(mark[str[i]])
     {
      if(!b[mark[str[i]]][str[i+1]-'a'+1])
       {
        out[mark[str[i]]]++;
        in[str[i+1]-'a'+1]++;
        a[mark[str[i]]][str[i+1]-'a'+1]=a[str[i+1]-'a'+1][mark[str[i]]]=1;
        ans++;
        b[mark[str[i]]][str[i+1]-'a'+1]=1;
       }
     }
    if(mark[str[i+1]])
     {
      if(!b[str[i]-'a'+1][mark[str[i+1]]])
       {
        in[mark[str[i+1]]]++;
        out[str[i]-'a'+1]++;
        a[mark[str[i+1]]][str[i]-'a'+1]=a[str[i]-'a'+1][mark[str[i+1]]]=1;
        ans++;
        b[str[i]-'a'+1][mark[str[i+1]]]=1;
       }
     }
    if(mark[str[i]]&&mark[str[i+1]])
     {
      if(!b[mark[str[i]]][mark[str[i+1]]])
       {
        in[mark[str[i+1]]]++;
        out[mark[str[i]]]++;
        a[mark[str[i+1]]][mark[str[i]]]=a[mark[str[i]]][mark[str[i+1]]]=1;
        ans++;
        b[mark[str[i]]][mark[str[i+1]]]=1;
       }
     }
   }
 
 for(i=1;i<=34;i++)
   if((in[i]||out[i])&&!hash[i])
    {
     flag=0;
     ff(i);
     if(flag==0)
      s++;
    }
 printf("%ld\n",ans+s+ss);
}

int main()
{
 long t;
 long i;
 
 freopen("2.in","r",stdin);
 freopen("2.out","w",stdout);
 
 mark['o']=0+27;
 mark['i']=1+27;
 mark['e']=2+27;
 mark['a']=3+27;
 mark['s']=4+27;
 mark['t']=5+27;
 mark['b']=6+27;
 mark['g']=7+27;
 
 scanf("%ld",&t);
 for(i=1;i<=t;i++)
   work(i);
   
 return 0;
}
