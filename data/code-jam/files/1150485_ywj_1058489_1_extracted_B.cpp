#include<stdio.h>
#include<algorithm>
using namespace std;

struct node{
	int p,v;
}tan[220];

int cmp(node a,node b){
	//if(a.p>b.p)
	return a.p>b.p;
}

bool pd(double time,double dis,int n){
	double pos=tan[0].p+time;
	for(int i=0;i<n;i++){
		double pos1=tan[i].p+time;
		if(pos>pos1)pos=pos1;
		pos-=tan[i].v*dis;
		if(pos+dis<tan[i].p){
			if(tan[i].p-(pos+dis)>time)return false;
		}
	}
	/*double pos1=tan[n-1].p+time;
	if(pos>pos1)pos=pos1;
	pos-=(tan[n-1].v-1)*dis;
	if(pos<tan[n-1].p){
		if(tan[n-1].p-pos>time)return false;
	}*/
	return true;
}



int main(){
	freopen("B1.in","r",stdin);
	freopen("B1.txt","w",stdout);
	int T,Case=1;
	int nc,dis;
	scanf("%d",&T);
	while(T--){
		scanf("%d%d",&nc,&dis);
		for(int i=0;i<nc;i++){
			scanf("%d%d",&tan[i].p,&tan[i].v);
		}
		sort(tan,tan+nc,cmp);
		double start=0.0,end=10.0,tmp,exp=0.1,EXP=0.1,mid;
		for(int i=1;i<18;i++){
			end*=10.0;
		}
		for(int i=0;i<7;i++){
			exp*=0.1;
		}
		for(int i=1;i<9;i++){
			EXP*=0.1;
		}
		int idx=1;
		while(end-start>exp){
			mid=(end+start)/2.0;
			if(pd(mid,dis,nc)){
				end=mid-EXP;
			}
			else {
				start=mid+EXP;
			}
			idx++;
			if(idx>1000)break;
		}
		printf("Case #%d: %.8lf\n",Case++,mid);
	}
	return 0;
}