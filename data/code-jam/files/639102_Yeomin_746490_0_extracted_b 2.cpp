#include <cstdio>
#include <algorithm>
#include <map>

using namespace std;

map<int, int> m;

int main()
{
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		int n;
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
		{
			int a, b;
			scanf("%d%d", &a, &b);
			m[a] = b;
		}

		int ret = 0;
		while (1)
		{
			int mx = 0, pt = 0;
			for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it)
			{
				if (mx < it->second)
				{
					mx = it->second; pt = it->first;
				}
			}
			if (mx < 2) break;
			m[pt] -= 2;
			m[pt - 1]++;
			m[pt + 1]++;
			++ret;
		}

		printf("Case #%d: %d\n", ++cases, ret);

		m.clear();
	}
}