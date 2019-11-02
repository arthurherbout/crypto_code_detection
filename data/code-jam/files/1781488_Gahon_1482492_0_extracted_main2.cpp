#include <stdio.h>
#include <iostream>
#include <set>
#include <math.h>
using namespace std;

double srd[2005],acs[2005];
double ai[255];

int main()
{
	int t,n,i,j,k,a,m;
	double d,abc1,abc2;
	FILE* in;
	FILE* out;

	in=fopen("D:/B-small-attempt0.in","r");
	out=fopen("D:/B-small-attempt0.out","w");
	fscanf(in,"%d",&t);
	
	for(i=1;i<=t;i++)
	{
		fprintf(out,"Case #%d:\n",i);
		
		fscanf(in,"%lf",&d);
		
		fscanf(in,"%d %d",&n,&a);
		
		
		m=0;
		for(j=0;j<n;j++){
			fscanf(in,"%lf %lf",&srd[j],&acs[j]);
			if(acs[i]>=d)m=i;
		}
		
		for(j=0;j<a;j++){
			fscanf(in,"%lf",&ai[j]);
		}
		
		if(n==2)
		abc1=(d-acs[0])*(srd[1]-srd[0])/(acs[1]-acs[0]);
		else abc1=-1;
		for(j=0;j<a;j++){
			abc2=(sqrt(2*ai[j]*d)/(ai[j]));
			if(abc1>abc2)fprintf(out,"%.8lf\n",abc1);
			else fprintf(out,"%.8lf\n",abc2);
		}

	}	
	fclose(in);
	fclose(out);
	return 0;
}

