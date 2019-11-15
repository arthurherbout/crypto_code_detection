#include<stdio.h>
#include<algorithm>
using namespace std;
int n;
pair<int,int> a[400];
int main(){
	int t,_,d;
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%d%d",&n,&d);
		for(int i=0; i<n; i++)
			scanf("%d%d",&a[i].first,&a[i].second);
		sort(a,a+n);
		double l=0,r=2e12;
		while(r-l>1e-7){
			double m=(l+r)/2,now=-1e18;
			bool flag=true;
			for(int i=0; i<n && flag; i++)
				for(int j=0; j<a[i].second && flag; j++)
				{
					now=max(now+d,a[i].first-m);
					if(now>a[i].first+m)
						flag=false;
				}
			if(flag)
				r=m;
			else
				l=m;
		}
		printf("Case #%d: %lf\n",t,l);
	}
	return 0;
}
