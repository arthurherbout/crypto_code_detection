#include<stdio.h>
#include<string.h>

int main()
{
int n;
int a;
char in[22];
int i,j,k,l;
scanf("%d",&n);
int arr[10];

for(i=0;i<n;i++)
{
for(j=0;j<22;j++)
{
in[j]=0;
}
for(j=0;j<10;j++)
{
arr[j]=0;
}


scanf("%s",in);
printf("Case #%d: ",i+1);
int len=strlen(in);
int point=0;

for(j=0;j<len;j++)
{
in[j]-='0';

}
for(j=len-1;j>=1;j--)
{
if(in[j-1]<in[j])
{
point=j-1;
break;
}
}

for(j=point;j<len;j++)
{
arr[in[j]]++;
}
int flag=0;

for(j=0;j<point;j++)
{
printf("%d",in[j]);
}

int min=100;
int minc=point;
flag=0;
for(j=point+1;j<len;j++)
{
if (in[j]<min&&in[j]>in[point])
{flag=1;
min=in[j];
minc=j;
}
}

if(minc!=point)
{int temp;
temp=in[point];
arr[in[minc]]--;
in[point]=in[minc];
in[minc]=temp;
printf("%d",in[point]);
point=point+1;

}

if(flag==0)
{
for(k=1;k<10;k++)
{
if(arr[k]>=1)
{printf("%d",k);
arr[k]--;
printf("0");
break;
}
}
for(k=0;k<10;k++)
{
int x=arr[k];
for(j=0;j<arr[k];j++)
{
printf("%d",k);
arr[k]--;
}
}

}

for(k=0;k<10;k++)
{
int x=arr[k];
for(j=0;j<x;j++)
{
printf("%d",k);
arr[k]--;}
}


printf("\n");
}
return 0;
}
