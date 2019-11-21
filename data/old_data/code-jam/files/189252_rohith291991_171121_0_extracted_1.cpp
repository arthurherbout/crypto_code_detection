#include<stdio.h>
#include<string.h>

int main()
{int n;
int i,j,k;

scanf("%d",&n);

char str[62];
int mark[512];

long long int ans; 
for(i=0;i<n;i++)
{
for(j=0;j<512;j++)
{
mark[j]=-1;
}
scanf("%s",str);
int len=strlen(str);

int var=1;
int base=0;
mark[str[0]]=1;

for(j=1;j<len;j++)
{


if(mark[str[j]]==-1)
{
if(base==1)
{base++;
}
mark[str[j]]=base;
base++;
}
}

if(base==1||base==0)
base=2;

ans=0;
int mul=1;


for(j=len-1;j>=0;j--)
{

ans+=mul*mark[str[j]];
mul*=base;
}
printf("Case #%d: %Ld\n",i+1,ans);

}



return 0;}
