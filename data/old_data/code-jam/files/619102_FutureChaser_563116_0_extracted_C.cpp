#include <stdio.h>
#include <string.h>
const int MAXN = 513;
int char2int[256];
int map[MAXN][MAXN];
int ans[MAXN];
bool used[MAXN][MAXN];
int ccount;
int solu[1000000][3];
int main()
{
	for (int i = 0; i < 10; ++i) char2int[i + '0'] = i;
	for (int i = 0; i < 6; ++i) char2int[i + 'A'] = i + 10;
	int cases;
	scanf("%d", &cases);
	for (int ca = 1; ca <= cases; ++ca)
	{
		printf("Case #%d: ", ca);
		int m, n;
		scanf("%d%d", &n, &m);
		memset(map, 0, sizeof(map));
		for (int i = 1; i <= n; ++i)
		{
			while (getchar() != '\n');
			for (int j = 1; j <= m / 4; ++j)
			{
				char temp;
				scanf("%c", &temp);
				for (int k = 1; k <= 4; ++k)
					map[i][(j - 1) * 4 + k] = ((char2int[temp] & (1 << (4 - k))) > 0);
			}
		}
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= m; ++j)
				map[i][j] ^= (i + j) % 2;
		for (int i = 1; i <= n; ++i)
			for (int j = 1; j <= m; ++j)
				map[i][j] += map[i - 1][j] + map[i][j - 1] - map[i - 1][j - 1];
		ccount = 0;
		for (int k = 512; k >= 1; --k)
		{
			for (int i = 1; i <= n - k + 1; ++i)
				for (int j = 1; j <= m - k + 1; ++j)
				{
					int x = i + k - 1, y = j + k - 1;
					int s = map[x][y] - map[x][j - 1] - map[i - 1][y] + map[i - 1][j - 1];
					if (s == 0 || s == k * k)
					{
						solu[ccount][0] = k, solu[ccount][1] = i, solu[ccount][2] = j;
						++ccount;
					}
				}
		}
		memset(used, 0, sizeof(used));
		memset(ans, 0, sizeof(ans));
		for (int k = 0; k < ccount; ++k)
		{
			int x = solu[k][1], y = solu[k][2], edge = solu[k][0];
			bool isCut = true;
			for (int i = x; i < x + edge; ++i)
			{
				for (int j = y; j < y + edge; ++j)
					if (used[i][j]) { isCut = false; break;}
				if (!isCut) break;
			}
			if (!isCut) continue;
			++ans[edge];
			for (int i =x; i < x + edge; ++i)
				for (int j = y; j < y + edge; ++j)
					used[i][j] = true;
		}
		int ccount = 0;
		for (int i = 1; i <= 512; ++i) if (ans[i] > 0) ++ccount;
		printf("%d\n", ccount);
		for (int i = 512; i > 0; --i)
			if (ans[i])
				printf("%d %d\n", i, ans[i]);
	}
	return 0;
}
