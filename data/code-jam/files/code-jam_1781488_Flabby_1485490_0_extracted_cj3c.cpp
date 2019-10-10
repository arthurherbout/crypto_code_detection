#include <cstdio>
#include <cstring>
#include <algorithm>
using namespace std;

const int MAXN = 110;
typedef long long lint;

lint n, m;
long long int a[MAXN], aid[MAXN];
long long b[MAXN], bid[MAXN];
long long asum[MAXN][MAXN];
long long bsum[MAXN][MAXN];

long long MAX(long long x, long long y)
{
	return x > y ? x : y;
}
long long MIN(long long x, long long y)
{
	return x < y ? x : y;
}

void pr(long long int a[][MAXN])
{
	puts("------------------------");
	for (int i = 0; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++)
			printf ("sum[%d][%d]=%3I64d ", i, j, a[i][j]);
		puts("");
	}
}

int main()
{
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("1.out", "w", stdout);

	int cs, csnum;
	scanf ("%d", &csnum);
	for (cs = 1; cs <= csnum; cs++)
	{
		scanf ("%I64d %I64d", &n, &m);

		for (int i = 0; i < n; i++)
			scanf ("%I64d %I64d", &a[i], &aid[i]);

		for (int i = 0; i < m; i++)
			scanf ("%I64d %I64d", &b[i], &bid[i]);
		
		memset(asum, 0, sizeof(asum));
		for (int i = n - 1; i >= 0; i--)
			for (int j = 1; j <= 100; j++)
				if (aid[i] == j)
					asum[i][aid[i]] = asum[i + 1][aid[i]] + a[i];
				else
					asum[i][j] = asum[i + 1][j];

		memset(bsum, 0, sizeof(bsum));
		for (int i = m - 1; i >= 0; i--)
			for (int j = 1; j <= 100; j++)
				if (bid[i] == j)
					bsum[i][bid[i]] = bsum[i + 1][bid[i]] + b[i];
				else
					bsum[i][j] = bsum[i + 1][j];
		
	//	pr(asum);
	//	pr(bsum);

		long long ans = 0LL;
		if (n == 1)
		{
			ans = MIN(asum[0][aid[0]], bsum[0][aid[0]]);
		}
		else if (n == 2)
		{
			long long mx = 0;
			for (int i = 0; i <= m; i++)
			{
				mx = MIN(bsum[i][aid[1]], a[1])
					+ MIN(bsum[0][aid[0]] - bsum[i][aid[0]], a[0]);
				ans = MAX(mx, ans);
			}
		}
		else if (n == 3)
		{

			long long int mx = 0;
			for (int i = 0; i <= m; i++)
				for (int j = i; j <= m; j++)
				{
					mx = MIN(bsum[j][aid[2]], a[2])
					 	+ MIN(bsum[i][aid[1]] - bsum[j][aid[1]], a[1])
						 + MIN(bsum[0][aid[0]] - bsum[i][aid[0]], a[0]);
					
					ans = MAX(mx, ans);
			//		printf ("i=%d j=%d\n", i, j);
			//		printf ("%I64d %I64d %I64d mx %I64d ans=%I64d\n", 
			//				MIN(bsum[j][aid[2]], a[2]),
			//				MIN(bsum[i][aid[1]] - bsum[j][aid[1]], a[1]),
			//				MIN(bsum[0][aid[0]] - bsum[i][aid[0]], a[0]),
			//				mx, ans);
				}
			ans = MAX(ans, MIN(asum[0][aid[0]], bsum[0][aid[0]]));
			ans = MAX(ans, MIN(asum[0][aid[1]], bsum[0][aid[1]]));
			ans = MAX(ans, MIN(asum[0][aid[2]], bsum[0][aid[2]]));
		}

		printf ("Case #%d: %I64d\n", cs, ans);
	}
	
	return 0;
}

