#include<stdio.h>
int n;
long long int calc_bad(long long int w){
	long long int better=w;
	long long int res=0;
	for(int i=0; i<n; i++){
		if(better==0)
			return res;
		better=(better-1)/2;
		res+=1LL<<(n-1-i);
	}
	return res;
}
long long int calc_good(long long int w){
	long long int worse=(1LL<<n)-1-w;
	long long int res=(1LL<<n)-1;
	for(int i=0; i<n; i++){
		if(worse==0)
			return res;
		worse=(worse-1)/2;
		res/=2;
	}
	return res;
}
int main(){
	int _;
	scanf("%d",&_);
	for(int t=1; t<=_; t++){
		long long int num;
		scanf("%d%I64d",&n,&num);--num;
		printf("Case #%d: ",t);
		long long int lo=0,hi=(1LL<<n)-1;
		while(lo!=hi){
			long long int mi=(lo+hi+1)/2;
			if(calc_bad(mi)<=num)
				lo=mi;
			else
				hi=mi-1;
		}
		printf("%I64d ",lo);

		lo=0,hi=(1LL<<n)-1;
		while(lo!=hi){
			long long int mi=(lo+hi+1)/2;
			if(calc_good(mi)<=num)
				lo=mi;
			else
				hi=mi-1;
		}
		printf("%I64d\n",lo);
	}
	return 0;
}