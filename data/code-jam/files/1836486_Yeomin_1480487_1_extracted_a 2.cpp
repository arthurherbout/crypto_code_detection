#include <cstdio>
#include <algorithm>
#include <vector>

using namespace std;

int t, cases;
int n, a[200], b;
bool nomore[200];
double lo, hi;

int main()
{
	scanf("%d", &t);
	while (t--)
	{
		scanf("%d", &n);
		b = 0;
		for (int i = 0; i < n; ++i)
		{
			scanf("%d", &a[i]);
			b += a[i];
			lo = min(lo, double(a[i]));
			hi = max(hi, double(a[i]));
			nomore[i] = false;
		}

		lo = 0; hi = 200;
		double med;
		for (int i = 0; i < 200; ++i)
		{
			med = (lo + hi) / 2.0;
			double rate = 0;
			for (int j = 0; j < n; ++j)
			{
				if (a[j] > med) continue;
				double more = med - a[j];
				rate += more / b;
			}
			if (rate > 1.f) hi = med;
			else lo = med;
		}

		printf("Case #%d:", ++cases);
		for (int i = 0; i < n; ++i)
		{
			double more = med - a[i];
			if (more < 0.0)
				printf(" 0.000000000");
			else
				printf(" %.9lf", more * 100.0 / b);
		}
		printf("\n");
	}
}