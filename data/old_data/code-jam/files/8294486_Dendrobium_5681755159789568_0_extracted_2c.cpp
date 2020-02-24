#include <cstdio>

#include <cstring>

using namespace std;

int e[105];
int s[105];
int map[105][105];

int main()
{
	int T;
	scanf("%d",&T);
	for (int t = 1; t <= T; t ++)
	{
		int n,q;
		scanf("%d%d",&n,&q);
		for (int i = 1; i <= n; i ++)
			scanf("%d%d",&e[i],&s[i]);
		for (int i = 1; i <= n ; i ++)
			for (int j = 1; j <= n; j ++)
				scanf("%d",&map[i][j]);
		double tim = 0;
		for (int i = 1; i <= q; i ++)
		{
			int u,v;
			scanf("%d%d",&u,&v);
			int nowe = e[u];
			int nows = s[u];
			for (int now = u; now <= v - 1; now ++)
			{
				if (nowe < map[now][now + 1] || nows < s[now])
				{
					nows = s[now];
					nowe = e[now];
				}
				tim += (double)map[now][now + 1] / nows;
				nowe -= map[now][now + 1];
			}
		}
		printf("Case #%d: %.6f\n",t,tim);
	}
	return 0;
}