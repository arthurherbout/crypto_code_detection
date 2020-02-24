#include<stdio.h>
#include<string.h>

const int maxn = 100;
int n,m;
char g[maxn][maxn];
char g2[maxn][maxn];
char used[maxn];
int id[maxn];
char ans;

void work() {
	int i,j,k;
	k = 1;
	for(i=0;i<m;i++)for(j=0;j<m;j++)
	{
		if(g2[i][j] != g[id[i]][id[j]]) k = 0;
	}
	if(k) ans = 1;
}

void dfs(int left)
{
	if(ans) return;
	if(left==0)
	{
		work();
		return;
	}
	int i,j;
	for(i=n-1; i>=0; i--)if(used[i]==0)
	{
		used[i] = 1;
		id[left-1] = i;
		dfs(left-1);
		used[i] = 0;
	}
}

int main() {
	int i,j,k;
	int cs,step;
	scanf("%d",&cs);
	for(step=1;step<=cs;step++)
	{
		printf("Case #%d: ", step);
		scanf("%d",&n);
		memset(g, 0, sizeof(g));
		for(i=1;i<n;i++)
		{
			scanf("%d%d",&j,&k);
			j--; k--;
			g[j][k] = g[k][j] = 1;
		}
		scanf("%d",&m);
		memset(g2, 0, sizeof(g2));
		for(i=1;i<m;i++)
		{
			scanf("%d%d",&j,&k);
			j--; k--;
			g2[j][k] = g2[k][j] = 1;
		}
		
		memset(used, 0, sizeof(used));
		ans = 0;
		dfs(m);
		if(ans) printf("YES\n"); else printf("NO\n");
	}
	return 0;
}
