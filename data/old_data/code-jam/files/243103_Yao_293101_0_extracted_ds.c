#include<stdio.h>
#include<string.h>
#include<algorithm>
using namespace std;

#define M 1000000007

long long A[20000000];
char tmp[16];
long long a,b;

int main() {
	int N,cs=0,i,j,k,p;
	long long t,ret,t1;

	for(scanf("%d",&N);N--;) {	
		scanf("%lld %lld",&a,&b);
		for(p=i=1;i<1000000;i++) {
			sprintf(tmp,"%d",i);
			k=strlen(tmp);
			for(j=0;j<k;j++) tmp[k+j]=tmp[k-j-1];
			tmp[k+k]='\0';
			sscanf(tmp,"%lld",&t);
			if (t>=a && t<=b) A[p++]=t; else if (t<a) continue;
			sprintf(tmp,"%d",i);
			k=strlen(tmp);
			for(j=1;j<k;j++) tmp[j+k-1]=tmp[k-j-1];
			tmp[k+k-1]='\0';
			sscanf(tmp,"%lld",&t);
			if (t>=a && t<=b) A[p++]=t; else if (t>b) break;
		}
		A[0]=a-1,A[p++]=b+1;
		sort(A,A+p);
		for(i=t=0;i+1<p;i+=2) t+=A[i+1]-A[i];
		ret=0;
		for(i=1,t1=0;i+1<p;i+=2) t1+=A[i+1]-A[i];
		ret=(b-a+2)%M*((b-a+1)%M)%M*((M+1)/2)%M-t%M*(t1%M)%M;
		ret%=M;
		if (ret<0) ret+=M;
		printf("Case #%d: %lld\n",++cs,ret);
	}
	return 0;
}
