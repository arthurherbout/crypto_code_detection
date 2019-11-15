#include <cstdio>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

int t, cases;
int n, a[20];

vector<int> v[2000001];

int main()
{
	scanf("%d", &t);
	while (t--)
	{
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
			scanf("%d", &a[i]);

		int found = -1, f2;
		for (int i = 1; i < (1 << n); ++i)
		{
			int x = 0;
			for (int j = 0; j < n; ++j)
			{
				if (i & (1 << j))
					x += a[j];
			}
			for (int j = 0; j < v[x].size(); ++j)
			{
				if ((v[x][j] | i) == (v[x][j] + i))
				{
					found = v[x][j];
					f2 = i;
					break;
				}
			}
			if (found != -1) break;
			v[x].push_back(i);
		}

		printf("Case #%d:\n", ++cases);
		if (found == -1)
			printf("Impossible\n");
		else
		{
			bool s = false;
			for (int j = 0; j < n; ++j)
			{
				if (found & (1 << j))
				{
					if (s) printf(" ");
					printf("%d", a[j]);
					s = true;
				}
			}
			printf("\n");
			
			s = false;
			for (int j = 0; j < n; ++j)
			{
				if (f2 & (1 << j))
				{
					if (s) printf(" ");
					printf("%d", a[j]);
					s = true;
				}
			}
			printf("\n");
		}

		for (int i = 0; i < 2000001; ++i)
			v[i].clear();
	}
}