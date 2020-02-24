#include <stdio.h>
#include <stdint.h>
typedef int64_t Z;
Z gcd(Z a,Z b){return b?gcd(b,a%b):a;}
Z find(Z P,Z Q,Z i,Z b){
	printf("%lld %lld %lld %lld\n",P,Q,i,b);
	if(i>b)return i;
	if(P>Q)return i;
	if(P<Q)return find(P,Q/2,i+1,b);
}
int main(void){
	Z P,Q,g,Ti,Tn,i,u,v,x,y;
	scanf("%lld",&Tn);
	for(Ti=1;Ti<=Tn;Ti++){
		scanf("%lld/%lld",&P,&Q);
		g=gcd(P,Q);
		P/=g;Q/=g;
		if(P==Q){i=1;goto ans;}
		x=Q;i=0;
		while(x){
			if(x&1){
				if(x>1)goto np;
				else break;
			}
			x>>=1;
			i++;
		}
		if(i>40)goto np;
		g=i;
		for(i=1;i<g;i++){
			Q>>=1;
			if(P>=Q)break;
		}
		if(i>g)goto np;
ans:		printf("Case #%lld: %lld\n",Ti,i);
		continue;
np:		printf("Case #%lld: impossible\n",Ti);
	}
	return 0;
}
