#include <stdio.h>
char Ex[100];
int K,N,M;
int W[30][26];
int Cnt[26];
int Ans[15];
int A[20][26];
void solve(int d)
{
		if (d>K) return;
		if (d>0)
		{
				//calc
				for (int i=0;i<=M;++i)
				{
						int t=1;
						for (int q=0;q<26;++q) for (int w=0;w<W[i][q];++w)
								t=(t*Cnt[q])%10009;
						Ans[d]=(Ans[d]+t)%10009;
				}
		}
		if (d+1>K) return;
		for (int q=0;q<N;++q)
		{
				for (int w=0;w<26;++w) Cnt[w]+=A[q][w];
				solve(d+1);
				for (int w=0;w<26;++w) Cnt[w]-=A[q][w];
		}
}
int main()
{
		int T;
		scanf("%d",&T);
		for (int kase=1;kase<=T;++kase)
		{
				fprintf(stderr,"%d\n",kase);
				scanf("%s %d",Ex,&K);
				scanf("%d",&N);
				M=0;
				for (int q=0;q<30;++q) for (int w=0;w<26;++w) W[q][w]=0;
				for (int q=0;Ex[q];++q)
				{
						if (Ex[q]=='+') M++;
						else W[M][Ex[q]-'a']++;
				}
				for (int q=0;q<N;++q)
				{
						for (int w=0;w<26;++w) A[q][w]=0;
						char tmp[100];
						scanf("%s",tmp);
						for (int w=0;tmp[w];++w) A[q][tmp[w]-'a']++;
				}
				for (int q=0;q<26;++q) Cnt[q]=0;
				for (int q=1;q<=K;++q) Ans[q]=0;
				solve(0);
				printf("Case #%d: ",kase);
				for (int q=1;q<=K;++q) printf("%d ",Ans[q]);
				printf("\n");
		}
		return 0;
}
