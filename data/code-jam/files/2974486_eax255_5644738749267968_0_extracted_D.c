#include <stdio.h>
#include <stdlib.h>
#include <string.h>
double nb[1000];
double kb[1000];
double nbv[1000];
double kbv[1000];
int kn,nn,knv,nnv;
void optNDW(double*r){
	int a,b,k;
	a=0;
	b=nnv;
	while(a<b){
		k=(a+b)>>1;
		if(nbv[k]>kbv[0])b=k;
		else a=k+1;//equality not tested as it is not allowed
	}
	if(a==nnv){
		a=0;
		r[0]=nbv[a];
		r[1]=kbv[knv-1]-1e-6;
	}else{
		r[0]=nbv[a];
		r[1]=1;
	}
	nnv--;
	memmove(nbv+a,nbv+a+1,(nnv-a)*sizeof(double));
}
void optNW(double*r){
	int a=0;
	*r=nbv[a];
	nnv--;
	memmove(nbv+a,nbv+a+1,(nnv-a)*sizeof(double));
}
void optKW(double N,double*r){
	int a,b,k;
	a=0;
	b=knv;
	while(a<b){
		k=(a+b)>>1;
		if(kbv[k]>N)b=k;
		else a=k+1;//equality not tested as it is not allowed
	}
	if(a==knv)a=0;
	*r=kbv[a];
	knv--;
	memmove(kbv+a,kbv+a+1,(knv-a)*sizeof(double));
}
int playDW(){
	double n[2],k;
	int r=0;
	memcpy(nbv,nb,nn*sizeof(double));
	memcpy(kbv,kb,kn*sizeof(double));
	knv=kn;
	nnv=nn;
	while(knv>0){
		if(nbv[0]>kbv[knv-1])return r+knv;
		if(nbv[nnv-1]<kbv[0])return r;
		optNDW(n);
		optKW(n[1],&k);
		if(k<n[0])r++;
	}
	return r;
}
int playW(){
	double n,k;
	int r=0;
	memcpy(nbv,nb,nn*sizeof(double));
	memcpy(kbv,kb,kn*sizeof(double));
	knv=kn;
	nnv=nn;
	while(knv>0){
		if(nbv[0]>kbv[knv-1])return r+knv;
		if(nbv[nnv-1]<kbv[0])return r;
		optNW(&n);
		optKW(n,&k);
		if(k<n)r++;
	}
	return r;
}
int cmp(const void*a,const void*b){
	if(*(double*)a<*(double*)b)return -1;
	else return 1;//equality not allowed
}
int main(){
	int T,i,a0,a1;
	scanf("%d",&T);
	for(i=1;i<=T;i++){
		scanf("%d",&nn);kn=nn;
		for(a0=0;a0<nn;a0++)scanf("%lf",nb+a0);
		for(a0=0;a0<nn;a0++)scanf("%lf",kb+a0);
		qsort(nb,nn,sizeof(double),cmp);
		qsort(kb,kn,sizeof(double),cmp);
		a0=playDW();
		a1=playW();
		printf("Case #%d: %d %d\n",i,a0,a1);
	}
	return 0;
}

