/*
Coder: Butterfly21
Task: Multi-base happiness
Source: Google Code Jam 2009 Round 1A
*/
#include<cstdio>
#include<memory>
int EF,T,b[10],dp[1000001][10],C,tm[100];
inline int gn(){
	int tmp=0;
	char c;
	while(1){
		c=getchar();
		if(c=='\n'){EF=1;break;}
		if(c==' ')break;
		tmp=tmp*10+c-48;
	}
	return tmp;
}
inline int conv(int N,int B){
	//printf("%d %d\n",N,B);
	int tmp=0,t2;
	while(N){
		t2=N%B;
		tmp+=t2*t2;
		N/=B;
	}
	if(~dp[tmp][B-1])return dp[tmp][B-1];
    dp[tmp][B-1]=0;
	return dp[tmp][B-1]=conv(tmp,B);
}
inline int ch(int N){
	for(int i=0;i<C;i++){
		if(dp[N][b[i]-1]==0)return 0;
		if(!conv(N,b[i]))return 0;
	}
	return 1;
}
int main(){
	freopen("A-small.in","r",stdin);
	freopen("A-small.out","w",stdout);
	scanf("%d\n",&T);
	memset(dp,-1,sizeof(dp));
	for(int i=0;i<10;i++)dp[1][i]=1,dp[0][i]=0;
	for(int i=1;i<=T;i++){
		EF=0;
		for(C=0;(!EF);C++)b[C]=gn();
		//for(int j=0;j<C;j++)printf("%d ",b[j]);
  		int chk=0,num=2;
		while(!chk)
			if(ch(num))chk=1;
			else num++;
		printf("Case #%d: %d\n",i,num);
	}
}
