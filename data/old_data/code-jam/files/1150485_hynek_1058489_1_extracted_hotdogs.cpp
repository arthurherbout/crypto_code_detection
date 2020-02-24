#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#define N 110

using namespace std;

int t, p, v, c, d, index;
double hranice, dalsi, cas;
vector < pair<int, int> > Vstup;

int main()
{
// 	freopen("hotdogs.in", "r", stdin);
	scanf("%d", &t);
	for(int e=1; e<=t; e++)
	{
		cas = 0;
		scanf("%d%d", &c, &d);
		Vstup.clear();
		for(int i=1; i<=c; i++)
		{
			scanf("%d%d", &p, &v);
			Vstup.push_back(make_pair(p, v));
		}
		sort(Vstup.begin(), Vstup.end());
// 		for(unsigned i=0; i<Vstup.size(); i++)
// 		{
// 			printf("%d:%d\n", Vstup[i].first, Vstup[i].second);
// 		}
		index = 0;
		hranice = Vstup[index].first + d;
		Vstup[index].second--;
		if(Vstup[index].second == 0)
			index++;
		while(index < (int)(Vstup.size()))
		{
			dalsi = Vstup[index].first;
			Vstup[index].second--;
// 			printf("h: %lf, d: %lf, c: %lf, index=%d, zbyva=%d\n",
// 				   hranice, dalsi, cas, index, Vstup[index].second);
			if(Vstup[index].second == 0)
				index++;
			if(hranice <= dalsi)
			{
				hranice = max(hranice + d, dalsi - cas + d);
			}
			else
			{
				if(dalsi + cas >= hranice)
				{
					hranice += d;
				}
				else
				{
					dalsi += cas;
					cas += (hranice - dalsi)/2;
					hranice += -(hranice - dalsi)/2 + d;
				}
			}
		}
		printf("Case #%d: %lf\n", e, cas);
	}
	return 0;
}
