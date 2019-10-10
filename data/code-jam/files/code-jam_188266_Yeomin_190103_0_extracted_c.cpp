#include <cstdio>
#include <algorithm>

using namespace std;

#define MAX_STEP 1000000

int pascal[11][11];
double table[2][11];
double res;
int n, c;

int main()
{
	pascal[0][0] = 1;
	for (int i = 1; i < 11; ++i)
	{
		pascal[i][0] = 1;
		for (int j = 1; j <= i; ++j)
			pascal[i][j] = pascal[i - 1][j - 1] + pascal[i - 1][j];
	}
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		res = 0.0;
		scanf("%d%d", &n, &c);
		table[0][c] = 1.0;
		int cur = 0, nex = 1, step = 1;
		double tot = double(pascal[n][c]);
		for(; step < MAX_STEP; ++step)
		{
			res += table[cur][n] * step;
			fill(table[nex], table[nex] + 11, 0.0);
			for (int i = c; i < n; ++i)
			{
				for (int j = c; j <= n; ++j)
				{
					int x = j - i;
					double s1 = double(pascal[i][c - x]);
					double s2 = double(pascal[n - i][x]);
					table[nex][j] += table[cur][i] * ((s1 * s2) / tot);
				}
			}
			swap(cur, nex);
		}

		printf("Case #%d: %lf\n", ++cases, res);
	}
}