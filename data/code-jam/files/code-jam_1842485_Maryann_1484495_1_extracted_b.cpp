#include<stdio.h>
#include<assert.h>
#include<algorithm>
using namespace std;
int n,w,h,a[1010],id[1010];
bool cmp(int x,int y){
	return a[x]<a[y];
}
int ox[1010],oy[1010];

struct Line{
	int x[1010],h[1010],L;
} line1,line2;
int main(){
	int _;
	scanf("%d",&_);
	for(int t=1; t<=_; t++){
		scanf("%d%d%d",&n,&w,&h);
		for(int i=0; i<n; i++){
			id[i]=i;
			scanf("%d",&a[i]);
			a[i]*=2;
		}
		sort(id,id+n,cmp);
		line1.L=1;
		line1.x[0]=-200000;
		line1.h[0]=-200000;
		line1.x[1]=w+200000;
		for(int i=n-1; i>=0; i--){
			int ii=id[i],len=a[ii],p=-1;
			for(int j=0; j<line1.L; j++)
				if(max(line1.x[j]+len/2,0)+len/2<=line1.x[j+1] && max(line1.x[j]+len/2,0)<=w && max(line1.h[j]+len/2,0)<=h){
					if(p==-1 || line1.h[j]<line1.h[p])
						p=j;
				}
			assert(p!=-1);
			ox[ii]=max(line1.x[p]+len/2,0);
			oy[ii]=max(line1.h[p]+len/2,0);
			line2.L=0;
			for(int j=0; j<p; j++){
				line2.x[line2.L]=line1.x[j];
				line2.h[line2.L]=line1.h[j];
				line2.L++;
			}
			line2.x[line2.L]=line1.x[p];
			line2.h[line2.L]=oy[ii]+len/2;
			line2.L++;
			if(ox[ii]+len/2<line1.x[p+1]){
				line2.x[line2.L]=ox[ii]+len/2;
				line2.h[line2.L]=line1.h[p];
				line2.L++;
			}
			for(int j=p+1; j<line1.L; j++){
				line2.x[line2.L]=line1.x[j];
				line2.h[line2.L]=line1.h[j];
				line2.L++;
			}
			line2.x[line2.L]=w+200000;
			line1=line2;
		}
		printf("Case #%d:",t);
		for(int i=0; i<n; i++)
			printf(" %d %d",ox[i],oy[i]);
		puts("");
	}
	return 0;
}
