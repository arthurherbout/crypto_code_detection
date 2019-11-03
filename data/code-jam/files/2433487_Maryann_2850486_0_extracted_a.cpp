#include<stdio.h>
#include<string.h>
#include<algorithm>
using namespace std;
long long int tot,a[37];
int n;
int main(){
	int _,t;
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%I64d%d",&tot,&n);
		memset(a,0,sizeof(a));
		for(int i=0; n--; i++)
			scanf("%I64d",&a[i]);
		sort(a,a+37);
		double res=0;
		for(long long int i=0; i<=1000; i++)
			for(int j=1; j<=37; j++){
				long long int cost=0,bet=0;
				bool flag=true;
				for(int k=0; k<j; k++){
					if(a[k]>i){
						flag=false;
						break;
					}
					cost+=i-a[k];
					bet+=i-a[k];
				}
				if(!flag)continue;
				for(int k=j; k<37; k++)
					cost+=max((i+1)-a[k],0LL);
				if(cost>tot)continue;
				res=max(res,36.0*bet/j-cost);
			}
		printf("Case #%d: %.10lf\n",t,res);
	}
	return 0;
}