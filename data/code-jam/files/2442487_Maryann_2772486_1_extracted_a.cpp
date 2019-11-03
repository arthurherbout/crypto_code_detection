#include<stdio.h>
#include<algorithm>
#include<utility>
#include<assert.h>
using namespace std;
#define mo 1000002013LL
#define ran 11111
int m;
long long int n;
long long int res;
int l;
pair<long long int,long long int> a[ran];
int len;
pair<long long int,long long int> lis[ran];
long long int add(long long int x,long long int y){
	return (x+=y)<mo?x:x-mo;
}
long long int mul(long long int x,long long int y){
	return (long long int)((long long int)x*y%mo);
}
long long int div2(long long int x){
	return x%2?(x+mo)/2:x/2;
}
long long int calc(long long int x){
	return div2(mul(add(n,n-x+1),x));
}
int main(){
	int _;
	scanf("%d",&_);
	for(int t=1; t<=_; t++){
		scanf("%I64d%d",&n,&m);
		res=0;
		l=0;
		for(int i=0; i<m; i++){
			long long int x,y,z;
			scanf("%I64d%I64d%I64d",&x,&y,&z);
			res=add(res,mul(calc(y-x),z));
			a[l++]=make_pair(x,-z);
			a[l++]=make_pair(y,z);
		}
		if(res)res=mo-res;
		sort(a,a+l);
		len=0;
		for(long long int i=0; i<l; i++)
			if(a[i].second<0)
				lis[len++]=make_pair(a[i].first,-a[i].second);
			else{
				long long int w=a[i].second;
				while(len>0){
					if(lis[len-1].second>w){
						res=add(res,mul(calc(a[i].first-lis[len-1].first),w));
						lis[len-1].second-=w;
						w=0;
						break;
					}
					res=add(res,mul(calc(a[i].first-lis[len-1].first),lis[len-1].second));
					w-=lis[len-1].second;
					--len;
				}
				assert(w==0);
			}
		assert(len==0);
		if(res)res=mo-res;
		printf("Case #%d: %I64d\n",t,res);
	}
	return 0;
}