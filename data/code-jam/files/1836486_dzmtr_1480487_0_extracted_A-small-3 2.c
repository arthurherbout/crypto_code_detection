#include <stdio.h>

int main()
{
	int T, t, i, j, n, a[10], A;

	scanf("%d", &T);

	for (t = 1; t <= T; ++t)
	{
		scanf("%d", &n);
		A = 0;
		for (i = 0; i < n; ++i)
		{
			scanf("%d", &a[i]);
			A += a[i];
		}
		printf("Case #%d: ", t);
		double ans[10];
		for (i = 0; i < n; ++i)
			ans[i] = 100.0 * (2.0 * A - n * a[i]) / (n * A);
		double m = 0.0;
		for (i = 0; i < n; ++i)
			if (ans[i] < m)
				m = ans[i];
		for (i = 0; i < n; ++i)
		{
			if (m < 0.0)
				printf("%.6f", ans[i] - m);
			else
				printf("%.6f", ans[i]);
			if (i == n - 1)
				putchar('\n');
			else
				putchar(' ');
		}
	}
	return 0;
}
