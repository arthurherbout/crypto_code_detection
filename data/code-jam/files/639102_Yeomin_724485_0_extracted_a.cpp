#pragma warning(disable:4996)

#include <cstdio>
#include <algorithm>

using namespace std;

typedef long long ll;

int s10[] = {1, 10, 100, 1000, 10000, 100000};
int isPrime[100000];
ll q[10];

int main()
{
	isPrime[2] = 1;
	for (int i = 3; i < 100000; i += 2)
	{
		isPrime[i] = 1;
		for (int j = 3; j < i; j += 2)
		{
			if (i % j == 0) 
			{
				isPrime[i] = 0;
				break;
			}
		}
	}
	int t, cases = 0;
	scanf("%d", &t);
	while (t--)
	{
		int d, k;
		scanf("%d%d", &d, &k);
		ll mx = 0;
		for (int i = 0; i < k; ++i)
		{
			scanf("%I64d", &q[i]);
			mx = max(q[i], mx);
		}
		ll nex = -1;
		if (k > 1)
		{
			for (int i = mx + 1; i < s10[d]; ++i)
			{
				if (isPrime[i] == 0) continue;
				for (ll a = 0; a < i; ++a)
				{
					ll y = (q[0] * a) % i;
					ll b = q[1] - y + i;
					ll x = q[0];
					bool use = true;
					for (int c = 1; c < k; ++c)
					{
						x = (a * x + b) % i;
						if (x != q[c])
						{
							use = false;
							break;
						}
					}
					if (use)
					{
						x = (a * x + b) % i;
						if (nex == -1 || nex == x)
							nex = x;
						else
						{
							nex = -2;
							break;
						}
					}
					if (nex == -2) break;
				}
				if (nex == -2) break;
			}
		}
		
		printf("Case #%d: ", ++cases);
		fprintf(stderr, "Case #%d: ", cases);
		if (nex < 0)
		{
			printf("I don't know.\n");
			fprintf(stderr, "I don't know. - nex: %d\n", nex);
		}
		else
		{
			printf("%I64d\n", nex);
			fprintf(stderr, "%I64d\n", nex);
		}
		fflush(stderr);
	}
}