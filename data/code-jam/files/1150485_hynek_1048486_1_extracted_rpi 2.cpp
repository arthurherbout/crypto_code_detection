#include <stdio.h>
#include <stdlib.h>
#include <vector>
#define N 110

using namespace std;

int n, t;
char ch;

double Wp[N], Owp[N], Oowp[N];
vector <int> Vit[N], Proh[N];

int main()
{
	//freopen("rpi.in", "r", stdin);
	scanf("%d", &t);
	for(int e=1; e<=t; e++)
	{
		scanf("%d", &n);
		for(int i=1; i<=n; i++)
		{
			scanf("%c", &ch);
			Vit[i].clear();
			Proh[i].clear();
			for(int j=1; j<=n; j++)
			{
				scanf("%c", &ch);
				if(ch == '1')
				{
					Vit[i].push_back(j);
				}
				if(ch == '0')
				{
					Proh[i].push_back(j);
				}
			}
		}
		for(int i=1; i<=n; i++)
		{
			Wp[i] = ((double)(Vit[i].size())) / (Vit[i].size() + Proh[i].size());
		}
		for(int i=1; i<=n; i++)
		{
			Owp[i] = 0;
			for(unsigned j=0; j<Vit[i].size(); j++)
			{
				Owp[i] += ((double)(Vit[Vit[i][j]].size())) / (Vit[Vit[i][j]].size() + Proh[Vit[i][j]].size() -1);
			}
			for(unsigned j=0; j<Proh[i].size(); j++)
			{
				Owp[i] += ((double)(Vit[Proh[i][j]].size()-1)) / (Vit[Proh[i][j]].size() + Proh[Proh[i][j]].size() -1);
			}
			Owp[i] /= (Vit[i].size() + Proh[i].size());
		}
		for(int i=1; i<=n; i++)
		{
			Oowp[i] = 0;
			for(unsigned j=0; j<Vit[i].size(); j++)
			{
				Oowp[i] += Owp[Vit[i][j]];
			}
			for(unsigned j=0; j<Proh[i].size(); j++)
			{
				Oowp[i] += Owp[Proh[i][j]];
			}
			Oowp[i] /= (Vit[i].size() + Proh[i].size());
		}
		printf("Case #%d:\n", e);
		for(int i=1; i<=n; i++)
		{
			//printf("%lf;%lf;%lf\n", Wp[i], Owp[i], Oowp[i]);
			printf("%lf\n", Wp[i]/4 + Owp[i]/2 + Oowp[i]/4);
		}
	}
	return 0;
}
