#include <cstdio>
#include <cstring>

using namespace std;

char map[105][105];
char oldmap[105][105];

int main()
{
	freopen("test.in","r",stdin);
	int T;
	scanf("%d",&T);
	for (int t = 1; t <= T; t ++)
	{
		memset(map,0,sizeof(map));
		memset(oldmap,0,sizeof(oldmap));
		int n,m,cnt = 0;
		scanf("%d%d",&n,&m);
		for (int i = 1; i <= m; i ++)
		{
			char c;
			int x,y;
			char s[15];
			scanf("%s",s);
			c = s[0];
			scanf("%d%d",&x,&y);
		//	printf("tyty %c\n",c);
			map[x][y] = c;
			oldmap[x][y] = c;
		}
		for (int i = 1; i <= n; i ++)
			for (int j = 1; j <= n; j ++)
			{
				if (!map[i][j] || map[i][j] == '+')
				{
					bool flag = 1;
					for (int p = 1; p <= n; p ++)
						for (int q = 1; q <= n; q ++)
							if ((i == p || j == q) && (map[p][q] == 'x' || map[p][q] == 'o'))
							{
								flag = 0;
								break;
							} 
					if (flag)
					{
						if (map[i][j])
						{
							map[i][j] = 'o';
						}
						else 
						{
							map[i][j] = 'x';
						}
					}
				}
				if (!map[i][j] || map[i][j] == 'x')
				{
					bool flag = 1;
					for (int p = 1; p <= n; p ++)
						for (int q = 1; q <= n; q ++)
							if ((i + j == p + q || i - j == p - q) && (map[p][q] == '+' || map[p][q] == 'o'))
							{
								flag = 0;
								break;
							}
					if (flag)
					{
						if (map[i][j])
						{
							map[i][j] = 'o';
						}
						else 
						{
							map[i][j] = '+';
						}
					}
				}
			}
		for (int i = 1; i <= n; i ++)
			for (int j = 1; j <= n; j ++)
				if (map[i][j] != oldmap[i][j])
					cnt ++;
		printf("Case #%d: %d %d\n",t,2 * n,cnt);
		for (int i = 1; i <= n; i ++)
			for (int j = 1; j <= n; j ++)
			{
				if (map[i][j] == 'o' && oldmap[i][j] != 'o')
					printf("o %d %d\n",i,j);
				else if (map[i][j] && !oldmap[i][j])
					printf("%c %d %d\n",map[i][j],i,j);
			}

	}
	return 0;
}
