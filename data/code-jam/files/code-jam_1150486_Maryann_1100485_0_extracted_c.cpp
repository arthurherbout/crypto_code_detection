#include<stdio.h>
long long int n;
bool f[1000010];
int pr[100000],sum,ans;
bool init(){
	for(int i=2; i<1000000; i++){
		if(!f[i])pr[sum++]=i;
		for(int j=0; j<sum; j++){
			int T=pr[j]*i;
			if(T>=1000000)break;
			f[T]=true;
			if(i%pr[j]==0)break;
		}
	}
}
int main(){
	int _,t;
	init();
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%I64d",&n);
		printf("Case #%d: ",t);
		if(n==1){
			puts("0");
			continue;
		}
		ans=1;
		for(int i=0; i<sum && 1LL*pr[i]*pr[i]<=n; i++){
			for(long long int x=pr[i]; x*pr[i]<=n; x*=pr[i])
				ans++;
		}
		printf("%d\n",ans);
	}
	return 0;
}
