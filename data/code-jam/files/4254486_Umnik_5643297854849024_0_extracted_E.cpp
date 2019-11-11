#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
using namespace std;

typedef long long ll;

const int N = 5050;
int n, d;
ll a[N];
ll b[N];

ll solve2()
{
	for (int i = 0; i < d; i++)
		b[i] = a[i] - a[i + d];
	for (int i = 0; i < d - 1; i++)
		if (abs(b[i] + b[i + 1]) % 2 != 0)
			return -1;
	ll p = 0;
	ll res = 0;
	for (int i = 0; i < d - 1; i++)
	{
		b[i + 1] += p;
		ll x = (b[i] - b[i + 1]) / 2;
		res += abs(x);
		b[i] -= x;
		b[i + 1] += x;
		p += x;
	}
	return res + abs(b[d - 1]);
}

void solve()
{
	scanf("%d%d", &n, &d);
	for (int i = 0; i < n; i++)
		scanf("%lld", &a[i]);
	for (int i = 0; i + 2 * d < n; i++)
	{
		if (a[i] != a[i + 2 * d])
		{
			printf("CHEATERS!\n");
			return;
		}
	}
	ll ans = 0;
	while(d > 0)
	{
		ll x = solve2();
		if (x == -1)
		{
			printf("CHEATERS!\n");
			return;
		}
		ans += x;
		for (int i = 0; i < d; i++)
			a[i] += a[i + d] + x;
		for (int i = 0; i < d; i++)
			a[i] /= 2;
		d /= 2;
	}
	printf("%lld\n", ans);
	return;
}

int main()
{
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t;
	scanf("%d", &t);
	for (int i = 1; i <= t; i++)
	{
		printf("Case #%d: ", i);
		solve();
	}

	return 0;
}