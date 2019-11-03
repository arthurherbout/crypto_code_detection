#include <stdio.h>
#include <iostream>
#include <set>
#include <math.h>
using namespace std;

double ti[2005],xi[2005];
double ai[255];

int main()
{
	int t,n,i,j,k,a,m;
	double d,t1,t2;
	FILE* in;
	FILE* out;

	in=fopen("B-small-attempt0.in","r");
	out=fopen("B-small-attempt0.out","w");
	
	fscanf(in,"%d",&t);
	
	for(i=1;i<=t;i++)
	{
		fprintf(out,"Case #%d:\n",i);
		
		fscanf(in,"%lf",&d);
		
		fscanf(in,"%d %d",&n,&a);
		
		//if(n==1){printf("cuowu!//////////////////////////");continue;}
		
		m=0;
		for(j=0;j<n;j++){
			fscanf(in,"%lf %lf",&ti[j],&xi[j]);
			if(xi[i]>=d)m=i;
		}
		
		for(j=0;j<a;j++){
			fscanf(in,"%lf",&ai[j]);
		}
		
		if(n==2)
		t1=(d-xi[0])*(ti[1]-ti[0])/(xi[1]-xi[0]);
		else t1=-1;
		for(j=0;j<a;j++){
			t2=(sqrt(2*ai[j]*d)/(ai[j]));
			if(t1>t2)fprintf(out,"%.8lf\n",t1);
			else fprintf(out,"%.8lf\n",t2);
		}

	}
	
	fclose(in);
	fclose(out);
	return 0;
}

