#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <algorithm>

using namespace std;

int T, x, s, r, t, n;
int b, e, w;
long double rychlost, metru, zbyvabezet, cas;
map <int, int> M;

int main()
{
	scanf("%d", &T);
	for(int E=1; E<=T; E++)
	{
		scanf("%d%d%d%d%d", &x, &s, &r, &t, &n);
		M.clear();
		for(int i=0; i<n; i++)
		{
			scanf("%d%d%d", &b, &e, &w);
			M[w] += e-b;
			x -= e-b;
		}
		M[0] += x;
		zbyvabezet = t;
		cas = 0;
		for(map<int, int>::iterator it = M.begin(); it != M.end(); it++)
		{
			rychlost = it->first;
			metru = it->second;
			if(zbyvabezet > 0 && metru / (rychlost+r) <= zbyvabezet)
			{
				cas += metru / (rychlost+r);
				zbyvabezet -= metru / (rychlost+r);
			}
			else
			{
				cas += zbyvabezet;
				metru -= (rychlost+r) * zbyvabezet;
				zbyvabezet = 0;
				cas += metru / (rychlost+s);
			}
		}
		printf("Case #%d: %Lf\n", E, cas);
	}
	return 0;
}
