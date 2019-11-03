#include<cstdio>
int T,N,K,tnmi[31];
void pre(){
	tnmi[0]=1;
	for(int i=1;i<31;i++)
	    tnmi[i]=tnmi[i-1]*2;
}
int main(){
	freopen("pA.in","r",stdin);
	freopen("pA.out","w",stdout);
	pre();
	scanf("%d",&T);
	for(int i=0;i<T;i++){
		printf("Case #%d: ",i+1);
		scanf("%d%d",&N,&K);
		if(K%tnmi[N]==tnmi[N]-1)puts("ON");
		else puts("OFF");
	}
}
