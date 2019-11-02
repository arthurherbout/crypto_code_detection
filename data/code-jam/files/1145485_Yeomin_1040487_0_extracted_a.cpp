#include <iostream>
#include <algorithm>

using namespace std;

typedef long long ll;

int t, d, g;
ll n;
int cases;

int main()
{
	scanf("%d", &t);
	while (t--)
	{
		scanf("%I64d%d%d", &n, &d, &g);

		bool ret = false;
		for (int i = 1; i <= n; ++i)
		{
			if (i * d % 100 != 0) continue;
			int wd = i * d / 100;
			for (int j = 0; j <= i * 1000; ++j)
			{
				if ((i + j) * g % 100 != 0) continue;
				int expWin = (i + j) * g / 100;
				int prevWin = expWin - wd;
				int prevLoss = j - prevWin;
//				printf("i: %d, j: %d, %d %d %d\n", i, j, expWin, prevWin, prevLoss);
				if (expWin >= 0 && prevWin >= 0 && prevLoss >= 0) ret = true;
			}
		}

		printf("Case #%d: ", ++cases);
		if (ret)
			printf("Possible\n");
		else
			printf("Broken\n");
	}
}