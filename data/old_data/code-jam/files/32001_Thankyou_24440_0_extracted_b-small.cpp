#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <math.h>
using namespace std;

int a, n, m;

int main()
{
	freopen("B-small.in", "r", stdin);
	freopen("B-small.out", "w", stdout);
	
	int cas;
	scanf("%d", &cas);
	for (int kth=1; kth<=cas; kth++)
	{
		scanf("%d%d%d", &n, &m, &a);
		int x1 = 0, y1 = 0;
		int x2, y2, x3, y3;
		bool find = false;
		for (x2=0; x2<=n; x2++)
		{
			for (y2=0; y2<=m; y2++)
			{
				for (x3=0; x3<=n; x3++)
				{
					for (y3=0; y3<=m; y3++)
					{
						if (abs(x2*y3 - x3*y2) == a)
						{
							find = true;
							break;	
						}
					}
					if (find)
						break;
				}
				if (find)
					break;
			}
			if (find)
				break;
		}
		printf("Case #%d: ", kth);
		if (!find)
			printf("IMPOSSIBLE\n");
		else
			printf("%d %d %d %d %d %d\n",x1, y1, x2, y2, x3, y3);
	}
	return 0;	
}
