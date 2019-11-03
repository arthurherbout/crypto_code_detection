#include <cstdio>
#include<stdlib.h>
#include<cmath>
#include<algorithm>
using namespace std;

#define DEBUG 0

int n, m, area;

int calArea(pair<int, int> a, pair<int, int> b, pair<int, int> c)
{
	return abs((a.first*b.second+b.first*c.second+c.first*a.second) - (b.first*a.second+c.first*b.second+a.first*c.second));
}

bool run(void)
{
	for(int x1=0;x1<=n;x1++)
		for(int y1=0;y1<=m;y1++)
			for(int x2=0;x2<=n;x2++)
				for(int y2=0;y2<=m;y2++)
				{
					pair<int, int> a,b,c;
					a.first=0, a.second=0;
					b.first=x1, b.second=y1;
					c.first=x2, c.second=y2;
					if(calArea(a,b,c)==area)
					{
						printf("0 0 %d %d %d %d\n", x1, y1, x2, y2);
						return 1;
					}
				}
	return 0;
}
int main()
{
	freopen("Round 2\\B-small-attempt2.in","r",stdin);
	//freopen("Round 2\\B-small.in","r",stdin);

#if !DEBUG
	freopen ("Round 2\\B-small-attempt2.out","w",stdout);
#endif

	int cases;

	scanf("%d\n", &cases);

	for(int i=1;i<=cases;i++)
	{
		scanf("%d %d %d", &n, &m, &area);
		printf("Case #%d: ", i);
		if(!run()) 
			printf("IMPOSSIBLE\n");

		//
	
	}
	

#if !DEBUG
	fclose (stdout);
#endif

	return 0;
}