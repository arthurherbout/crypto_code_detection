/*
Coder: Butterfly21
Task: Stock Charts
Source: Google Code Jam 2009 Round2 Problem C
*/
#include<cstdio>
#include<memory>
#include<algorithm>
int T,N,K,picked[101],cnt,crs[101][101],dp[101],pre[1001];
struct stock{
	long long P[30];
}S[101];
bool operator<(stock A,stock B){
	for(int i=0;i<K;i++)
	    if(A.P[i]!=B.P[i])return A.P[i]<B.P[i];
	return 0;
}
int cross(stock A,stock B){
	//printf("%d\n",K);
	//for(int i=0;i<K;i++){
	//    if(A.P[i]>=B.P[i])return 1;
	//}
	for(int i=1;i<K;i++){
		long long tmp=(A.P[i]-B.P[i])*(A.P[i-1]-B.P[i-1]);
	    if(tmp<=0)return 1;
	}
	return 0;
}
inline int min(int a,int b){return a<b?a:b;}
int main(){
	freopen("C-large.in","r",stdin);
	freopen("C-large.out","w",stdout);
	scanf("%d",&T);
	for(int i=0;i<T;i++){
        memset(picked,0,sizeof(picked));
        memset(crs,0,sizeof(crs));
        cnt=0;
		scanf("%d%d",&N,&K);
		for(int j=0;j<N;j++)
		    for(int k=0;k<K;k++)scanf("%I64d",&S[j].P[k]);
		std::sort(S,S+N);
		for(int j=0;j<N;j++)
		    for(int k=0;k<N;k++)
		        crs[j][k]=crs[k][j]=cross(S[j],S[k]);
		//puts("JIZZ");
		for(int j=0;j<N;j++){
			//printf("%d\n",j);
			if(picked[j])continue;
			cnt++;
			for(int k=0;k<N;k++)dp[k]=1,pre[k]=-1;
			for(int k=j+1;k<N;k++){
				if(picked[k])continue;
				for(int z=k+1;z<N;z++){
					if(picked[z])continue;
					if(crs[k][z])continue;
					if(dp[k]+1>dp[z])dp[z]=dp[k]+1,pre[z]=k;
				}
			}
			//puts("JIZZ");
			int MM=0,number=-1;
			for(int k=j+1;k<N;k++){
				if(picked[k])continue;
				if(crs[j][k])continue;
			    if(dp[k]>MM)number=k,MM=dp[k];
				//printf("%d ",dp[k]);
				
			}//putchar('\n');
			for(;number!=-1;number=pre[number])picked[number]=1;
			picked[j]=1;
		}
		int cnt2=0;
		memset(picked,0,sizeof(picked));
		for(int j=0;j<N;j++){
			//printf("%d\n",j);
			if(picked[j])continue;
			cnt2++;
			for(int k=0;k<N;k++)dp[k]=1,pre[k]=-1;
			for(int k=j+1;k<N;k++){
				if(picked[k])continue;
				for(int z=k+1;z<N;z++){
					if(picked[z])continue;
					if(crs[k][z])continue;
					if(dp[k]+1>=dp[z])dp[z]=dp[k]+1,pre[z]=k;
				}
			}
			//puts("JIZZ");
			int MM=0,number=-1;
			for(int k=j+1;k<N;k++){
				if(picked[k])continue;
				if(crs[j][k])continue;
			    if(dp[k]>MM)number=k,MM=dp[k];
				//printf("%d ",dp[k]);

			}//putchar('\n');
			for(;number!=-1;number=pre[number])picked[number]=1;
			picked[j]=1;
		}
		printf("Case #%d: %d\n",i+1,min(cnt,cnt2));
	}
}
