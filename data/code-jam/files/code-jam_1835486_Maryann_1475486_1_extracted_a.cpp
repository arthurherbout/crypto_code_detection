#include<stdio.h>
#include<algorithm>
using namespace std;
int w[10000],l[10000],p[10000],n,id[10000];
bool cmp(int x,int y){
	int r=100*l[x]+p[x]*l[y]-(100*l[y]+p[y]*l[x]);
	if(r!=0)return r<0;
	return x<y;
}
int main(){
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
