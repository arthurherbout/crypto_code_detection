#include <cstdio>
#include <algorithm>

using namespace std;

int main()
{
	int t, cases = 0;
	scanf("%d", &t);

	while (t--)
	{
		int n;
		int a[1000];
		int used[1000];
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
		{
			scanf("%d", &a[i]);
			--a[i];
		}
		fill(used, used + n, 0);

		int res = 0;
		for (int i = 0; i < n; ++i)
		{
			if (used[i] || a[i] == i) continue;
			used[i] = 1;
			int chain = 1;
			int x = i;
			while (true)
			{
				x = a[x];
				if (used[x]) break;
				used[x] = 1;
				++chain;
			}
			res += chain;
			fprintf(stderr, "chain: %d\n", chain);
		}
		printf("Case #%d: %d.000000\n", ++cases, res);
	}
}