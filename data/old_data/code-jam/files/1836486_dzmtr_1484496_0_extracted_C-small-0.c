#include <stdio.h>

int main()
{
	int T, t, i;
	scanf("%d", &T);
	int a[20];
	for (t = 1; t <= T; ++t)
	{
		int n;
		scanf("%d", &n);
		for (i = 0; i < n; ++i)
			scanf("%d", &a[i]);
		int b[20] = {};
		int found = 0;
		do
		{
			i = 0;
			while (i < n && b[i] == 2)
			{
				b[i] = 0;
				++i;
			}
			if (i < n)
				++b[i];
			else
				break;


			int x = 0, y = 0, k = 0, m = 0;

			for (i = 0; i < n; ++i)
				if (b[i] == 1)
					++k, x += a[i];
				else if (b[i] == 2)
					++m, y += a[i];
			if (k > 0 && m > 0 && x == y)
			{
				printf("Case #%d:\n", t);
				for (i = 0; i < n; ++i)
					if (b[i] == 1)
					{
						printf("%d", a[i]);
						--k;
						if (k > 0)
							putchar(' ');
						else
						{
							putchar('\n');
							break;
						}
					}
				for (i = 0; i < n; ++i)
					if (b[i] == 2)
					{
						printf("%d", a[i]);
						--m;
						if (m > 0)
							putchar(' ');
						else
						{
							putchar('\n');
							break;
						}
					}
				found = 1;
			}
		}
		while (!found);
		if (!found)
			printf("Case #%d:\nImpossible\n", t);
	}
	return 0;
}
