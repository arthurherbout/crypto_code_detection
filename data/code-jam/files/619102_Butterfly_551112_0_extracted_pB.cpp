#include<cstdio>
int T,C;
long long L,P,R1,R2,ans,tmp;
int main(){
	freopen("pB.in","r",stdin);
	freopen("pB.out","w",stdout);
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		scanf("%I64d%I64d%d",&L,&P,&C);
		R1=P/L,R2=ans=0;
		if(P%L)R1++;
		//printf("%I64d\n",R1);
		for(tmp=1;tmp<R1;tmp*=C,R2++);
		//printf("%I64d\n",R2);
		for(tmp=1;tmp<R2;tmp*=2,ans++);
		printf("Case #%d: %I64d\n",i+1,ans);
	}
}
