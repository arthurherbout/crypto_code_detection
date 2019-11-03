#include <stdio.h>
#include <iostream>
#include <set>
#include <math.h>
using namespace std;

long long startC(int xa,int xb);



int ai,bi;


struct node
{
	int kind;
	long long num;
};

node a[10],b[110];

int main()
{
	FILE* in;
	FILE* out;
	in=fopen("C-small-attempt0.in","r");
	out=fopen("C-small-attempt0.out","w");
	
	long long j,ans;
	int i,t;
	
	fscanf(in,"%d",&t);
	for (j=1;j<=t;j++)
	{
		fprintf(out,"Case #%d: ",j);
		fscanf(in,"%d%d",&ai,&bi);
		
		for (i=1;i<=ai;i++)
		fscanf(in,"%lld%d",&a[i].num,&a[i].kind);
		for (i=1;i<=bi;i++)
		fscanf(in,"%lld%d",&b[i].num,&b[i].kind);

		ans=startC(1,1); 
		fprintf(out,"%lld\n",ans);
	}
}


long long startC(int xa,int xb)
{
	if (xb>bi||xa>ai) return 0;
	if (a[xa].kind==b[xb].kind)
	{
		if (a[xa].num>b[xb].num)
		{
			long long t=b[xb].num;
			a[xa].num-=t;
			b[xb].num=0;
			return t+startC(xa,xb+1);
		}

		if (a[xa].num==b[xb].num)
		{
			long long t=b[xb].num;
			a[xa].num-=t;
			b[xb].num=0;
			return t+startC(xa+1,xb+1);
		}


		if (a[xa].num<b[xb].num)
		{
			long long t=a[xa].num;
			b[xb].num-=t;
			a[xa].num=0;
			return t+startC(xa+1,xb);
		}
	}
	else
	{
		int i;
		node aaa[4],bbb[101];
		for (i=1;i<=ai;i++)
		{aaa[i].kind=a[i].kind;aaa[i].num=a[i].num;}
		
		for (i=1;i<=bi;i++)
		{bbb[i].kind=b[i].kind;bbb[i].num=b[i].num;}

		long long t1=startC(xa+1,xb);

		for (i=1;i<=ai;i++)
		{a[i].kind=aaa[i].kind;a[i].num=aaa[i].num;}
		
		for (i=1;i<=bi;i++)
		{b[i].kind=bbb[i].kind;b[i].num=bbb[i].num;}

		long long t2=startC(xa,xb+1);
		
		if (t1>=t2) return t1;
		else return t2;
	}
}