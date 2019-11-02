//http://code.google.com/codejam/
#include<cstdio>
int num[100];
bool f[100][100];
bool s[100][100];
bool mark[100];
bool ok,n,m;
void dfs(int dep)
{
	int i,j;
	if(dep==m)
	{
		bool ss=1;
		for(i=0;ss&&(i<m);i++)
			for(j=0;ss&(j<m);j++)
				if(s[i+1][j+1]!=f[num[i]][num[j]])
				{
					ss=0;
					break;
				}
		if(ss)ok=1;
		return;
	}
	for(i=1;i<=n;i++)
	if(mark[i]==0)
	{
		num[dep]=i;
		mark[i]=1;
		dfs(dep+1);
		mark[i]=0;
	}
}
int main()
{	
	int t,cc,i,j,a,b;
	freopen("D-small-attempt2.in","r",stdin);
	freopen("d.out","w",stdout);
	scanf("%d",&t);
	for(cc=1;cc<=t;cc++)
	{
		scanf("%d",&n);
		for(i=1;i<=n;i++)
			for(j=1;j<=n;j++)f[i][j]=0;
		for(i=1;i<n;i++)
		{
			scanf("%d%d",&a,&b);
			f[a][b]=f[b][a]=1;
		}
		scanf("%d",&m);
		for(i=1;i<=m;i++)
			for(j=1;j<=m;j++)
				s[i][j]=0;
		for(i=1;i<m;i++)
		{
			scanf("%d%d",&a,&b);
			s[a][b]=s[b][a]=1;
		}
		ok=0;
		for(i=1;i<=n;i++)mark[i]=0;
		dfs(0);
		printf("Case #%d: ",cc);
		if(ok)printf("YES\n");
		else printf("NO\n");
	}
	return 0;
}
