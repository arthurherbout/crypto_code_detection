//Author: Fluorine
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <string>
#include <vector>
using namespace std;

long double calc(int,int);
long double p,q;
int tot,m,x;
double tp;

int main(){
	scanf("%d",&tot);
	for (int cases=0;cases<tot;++cases){
		scanf("%d%lf%d",&m,&tp,&x);
		p=(long double)tp;
		q=1-p;
		printf("Case #%d: %.7lf\n",cases+1,(double)calc(x,m));
	}
	return 0;
}

long double calc(int x,int m){
	long double ret=0;
	if (x>=1000000) return 1;
	if (x<=0) return 0;
	if (m<=0) return 0;
	int up=1000000;
	for (int i=1;i<m;++i){
		if (up%2==0) up/=2;
		else up=up/2+1;
	}
	//printf("%d %d\n",x,up);
	if (x*2<up) return 0;
	int delta;
	if (x<up) delta=up-x;
	else{
		delta=x-up;
		if (1000000-x<delta) delta=1000000-x;
	}
	ret+=calc(x-delta,m-1)*q;
	ret+=calc(x+delta,m-1)*p;
	//printf("%d %d %.6lf\n",x,m,(double)ret);
	return ret;
}
