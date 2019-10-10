#include<stdio.h>
#include<string.h>
char s[55];
double res;
int n,mask;
int f[1<<20],t;
double d[1<<20];
double dp(int mask){
	if(f[mask]==t)return d[mask];
	f[mask]=t;
	double&res=d[mask];
	res=0;
	if(mask==0)return res=.0;
	for(int i=0; i<n; i++)
		if(mask&(1<<i)){
			double qq=dp(mask-(1<<i));
			res+=n+qq;
			for(int j=(i+n-1)%n,n2=n-1; (mask&(1<<j))==0; j=(j+n-1)%n,n2--)
				res+=n2+qq;
		}
	return res/=n;
}
int main(){
	int _;
	scanf("%d",&_);
	for(t=1; t<=_; t++){
		scanf("%s",s);
		n=strlen(s);
		mask=0;
		for(int i=0; i<n; i++)
			if(s[i]=='.')
				mask|=1<<i;
		printf("Case #%d: %.10lf\n",t,dp(mask));
	}
	return 0;
}