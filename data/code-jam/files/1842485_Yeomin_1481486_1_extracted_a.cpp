#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

typedef long long ll;

int t, cases;
int n;
ll v[10000], l[10000];
ll to;
ll d[10000];

bool solve()
{
	d[0] = v[0] * 2;
	if (d[0] >= to)
		return true;
	for (int i = 1; i < n; ++i)
	{
		d[i] = 0;
		for (int j = 0; j < i; ++j)
		{
			if (d[j] >= v[i])
			{
				ll next = min(v[i] - v[j], l[i]);
				d[i] = max(d[i], v[i] + next);
				if (d[i] >= to)
					return true;
			}	
		}
	}

	return false;
}

int main()
{
	scanf("%d", &t);
	while(t--)
	{
		scanf("%d", &n);
		for (int i = 0; i < n; ++i)
			scanf("%I64d%I64d", &v[i], &l[i]);
		scanf("%I64d", &to);

		printf("Case #%d: ", ++cases);
		if (solve())
			printf("YES\n");
		else
			printf("NO\n");
	}
}