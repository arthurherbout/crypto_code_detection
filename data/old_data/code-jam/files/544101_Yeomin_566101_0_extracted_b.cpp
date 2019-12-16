#pragma warning (disable:4996)

#include <iostream>
#include <cstdio>
#include <algorithm>

using namespace std;

int t, cases;
int d, i, m, n, a[100];
int table[100][256];

int main()
{
//	freopen("B-small-attempt0.in", "r", stdin);
	scanf("%d", &t);
	while (t--)
	{
		scanf("%d%d%d%d", &d, &i, &m, &n);
		for (int q = 0; q < n; ++q)
			scanf("%d", &a[q]);
		fill(table[0], table[n], 1000000000);
		table[0][a[0]] = 0;
		int r = d * n;
		if (d == 0 || (i == 0 && m != 0))
		{
			r = 0;
		}
		else
		{
			for (int q = 0; q < n; ++q)
			{
				if (q != 0)
				{	// insert
					int w = a[q];
					if (m != 0)
						for (int e = 0; e < 256; ++e)
						{
							int y;
							y = abs(w - e) / m + (abs(w - e) % m != 0);

							if (y < 2) y = 0;
							else y = (y - 1) * i;
							table[q][w] = min(table[q][w], table[q - 1][e] + y);
						}
				}
				for (int w = 0; w < 256; ++w)
				{
					if (q == 0)
					{
						table[q][w] = abs(a[q] - w);
					}
					else
						table[q][w] = abs(a[q] - w) + table[q - 1][w];	// modify

					// insert and modify
					if (q != 0 && m != 0)
					{
						for (int e = 0; e < 256; ++e)
						{
							int y = abs(w - e);
							if (y <= m) y = 0;
							else y = (y - 1) / m * i;
							// insert and modify
							table[q][w] = min(table[q][w], table[q - 1][e] + y + abs(a[q] - w));
							// insert and delete
							table[q][w] = min(table[q][w], table[q - 1][e] + y + i + d);
						}
					}

					int X = 0;
					for (int r = q - 1; r >= 0; --r)
					{	// delete		
						X += d;
						/*
						if (m != 0)
						{	// delete and insert (?!)
							for (int e = 0; e < 256; ++e)
							{
								int y = abs(w - e);
								if (y <= m) y = 0;
								else y = (y - 1) / m * i;
								table[q][w] = min(table[q][w], table[r][e] + X + y + i);
							}
						}
						*/
						table[q][w] = min(table[q][w], table[r][w] + X);
						// delete and modify
						table[q][w] = min(table[q][w], table[r][w] + X - d + abs(a[q] - w));
					}
					X += d;
					// delete all and insert w.
					table[q][w] = min(table[q][w], X + i);

//					printf("%d ", table[q][w]);
				}
//				printf("\n");
			}
		}

		for (int i = 0; i <= 255; ++i)
			r = min(r, table[n - 1][i]);
		printf("Case #%d: %d\n", ++cases, r);
	}
}