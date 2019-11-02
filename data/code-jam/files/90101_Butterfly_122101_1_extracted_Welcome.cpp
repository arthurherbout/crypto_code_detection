/*
Coder: Butterfly21
Task: Welcome To Code Jam
Source: Google Code Jam 2009 Qualification Round Problem C
*/
#include<cstdio>
#include<cstring>
int dp[501][21]={0},N;
char line[501],pat[]="welcome to code jam";
int main(){
	freopen("C-large.in","r",stdin);
	freopen("C-large.out","w",stdout);
	scanf("%d",&N);
	getchar();
	for(int i=0;i<N;i++){
		gets(line);
		memset(dp,0,sizeof(dp));
		int L1=strlen(line),L2=strlen(pat);
		if(line[0]=='w')dp[0][0]=1;
		for(int j=1;line[j];j++){
			dp[j][0]=dp[j-1][0];
			if(line[j]=='w')dp[j][0]=(dp[j][0]+1)%10000;
		}
		for(int j=1;pat[j];j++){
			if((dp[j-1][j-1])&&(line[j]==pat[j]))dp[j][j]=dp[j-1][j-1];
		    for(int k=j+1;line[k];k++){
				dp[k][j]=dp[k-1][j];
				if(line[k]==pat[j])dp[k][j]=(dp[k][j]+dp[k-1][j-1])%10000;
			}
		}
		/*for(int j=0;j<L2;j++){
		    for(int k=0;k<L1;k++)printf("%d ",dp[k][j]);
			putchar('\n');
		}*/
		printf("Case #%d: %04d\n",i+1,dp[L1-1][L2-1]);
	}
	//scanf("  ");
}
