#include<stdio.h>
#include<algorithm>
using namespace std;
int w[10000],l[10000],p[10000],n,id[10000];
double calc(int a,int b,int c,int d){
	return (10000*a+100*b*c+.0)/(b*d);
}
bool cmp(int x,int y){
	return calc(l[x],p[x],l[y],p[y])<
		calc(l[y],p[y],l[x],p[x]);
}
int main(){
	freopen("D:\\A-small-attempt0.in","r",stdin);
	freopen("D:\\A-small-attempt0.out","w",stdout);
	int _;
	scanf("%d",&_);
	for(int t=1; t<=_; t++){
		scanf("%d",&n);
		for(int i=0; i<n; i++)
			scanf("%d",&l[i]);
		for(int i=0; i<n; i++){
			scanf("%d",&p[i]);
			p[i]=100-p[i];
			id[i]=i;
		}
		sort(id,id+n,cmp);
		printf("Case #%d:",t);
		for(int i=0; i<n; i++)
			printf(" %d",id[i]);
		puts("");
	}
	return 0;
}