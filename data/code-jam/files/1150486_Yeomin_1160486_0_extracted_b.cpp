#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <algorithm>

using namespace std;

int t, cases;
int table[10][10];
int x, y, d;
int ret;

void solve()
{
	for (int q = min(x, y); q >= 3; --q)
	{
		for (int i = 0; i <= x - q; ++i)
		{
			for (int j = 0; j <= y - q; ++j)
			{
				int _x = 0, _y = 0;
				for (int w = 0; w < q; ++w)
				{
					int ww = w * 2 + 1;
					for (int e = 0; e < q; ++e)
					{
						if ((w == 0 && e == 0) || (w == 0 && e == q - 1) || (w == q - 1 && e == 0) || (w == q - 1 && e == q - 1))
							continue;
						int ee = e * 2 + 1;
						_x += table[i + w][j + e] * (ww - q);
						_y += table[i + w][j + e] * (ee - q);
					}
				}
				if (_x == 0 && _y == 0)
				{
					ret = q;
					return;
				}
			}
		}
	}
}

int main()
{
	scanf("%d", &t);
	while (t--)
	{
		scanf("%d%d%d", &x, &y, &d);
		for (int i = 0; i < x; ++i)
		{
			char cc[100];
			scanf("%s", cc);
			for (int j = 0; j < y; ++j)
			{
				table[i][j] = cc[j] - '0' + d;
			}
		}
		ret = -1;
		solve();
		printf("Case #%d: ", ++cases);
		if (ret == -1)
			printf("IMPOSSIBLE\n");
		else
			printf("%d\n", ret);
	}
}