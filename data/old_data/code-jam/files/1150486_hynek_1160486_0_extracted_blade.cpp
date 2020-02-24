#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <algorithm>
#define N 550

using namespace std;

char ch;
int t, r, c, d, ans;
long long icko, jcko, ivaha, jvaha;
int Pole[N][N];

int main()
{
	scanf("%d", &t);
	for(int e=1; e<=t; e++)
	{
		scanf("%d%d%d", &r, &c, &d);
		for(int i=1; i<=r; i++)
		{
			ch = getc(stdin);
			for(int j=1; j<=c; j++)
			{
				ch = getc(stdin);
				Pole[i][j] = ch - '0';
			}
		}
		ans = 0;
		for(int i=1; i<=r; i++)
		{
			for(int j=1; j<=c; j++)
			{
				for(int k=3; i+k-1<=r && j+k-1<=c; k++)
				{
					icko = 0;
					jcko = 0;
					for(int ii=i; ii<i+k; ii++)
					{
						for(int jj=j; jj<j+k; jj++)
						{
							if((ii==i || ii==i+k-1) && (j==jj || jj==j+k-1))
								continue;
							icko += Pole[ii][jj] * ((2*ii) - (i+i+k-1));
							jcko += Pole[ii][jj] * ((2*jj) - (j+j+k-1));
						}
					}
					//printf("pro [%d,%d] k=%d je icko %lld a jcko %lld\n", i, j, k, icko, jcko);
					if(icko == 0 && jcko == 0)
					{
						ans = max(ans, k);
					}
				}
			}
		}
		if(ans)
			printf("Case #%d: %d\n", e, ans);
		else
			printf("Case #%d: IMPOSSIBLE\n", e);
	}
	return 0;
}

/*
for(int i=1; i<=r; i++)
		{
			for(int j=1; j<=c; j++)
			{
				Suma[i][j] = Pole[i][j] + Suma[i-1][j] + Suma[i][j-1] - Suma[i-1][j-1];
			}
		}
ans = 0;
		for(int k=min(r,c); k>=3 && ans==0; k--)
		{
			printf("zkousim k=%d..\n", k);
			for(int i=1; i+k-1<=r && ans==0; i++)
			{
				for(int j=1; j+k-1<=c && ans==0; j++)
				{
					if(k%2 == 0)
					{
horni = Suma[i+k/2-1][j+k-1] - Suma[i-1][j+k-1] - Suma[i+k/2-1][j-1] + Suma[i-1][j-1];
horni -= Pole[i][j] + Pole[i][j+k-1];
dolni = Suma[i+k-1][j+k-1] - Suma[i+k/2-1][j+k-1] - Suma[i+k-1][j-1] + Suma[i+k/2-1][j-1];
dolni -= Pole[i+k-1][j] + Pole[i+k-1][j+k-1];
levy = Suma[i+k-1][j+k/2-1] - Suma[i+k-1][j-1] - Suma[i-1][j+k/2-1] + Suma[i-1][j-1];
levy -= Pole[i][j] + Pole[i+k-1][j];
pravy = Suma[i+k-1][j+k-1] - Suma[i+k-1][j+k/2-1] - Suma[i-1][j+k-1] + Suma[i-1][j+k/2-1];
pravy -= Pole[i][j+k-1] + Pole[i+k-1][j+k-1];
					}
					else
					{
horni = Suma[i+k/2-1][j+k-1] - Suma[i-1][j+k-1] - Suma[i+k/2-1][j-1] + Suma[i-1][j-1];
horni -= Pole[i][j] + Pole[i][j+k-1];
dolni = Suma[i+k-1][j+k-1] - Suma[i+k/2][j+k-1] - Suma[i+k-1][j-1] + Suma[i+k/2][j-1];
dolni -= Pole[i+k-1][j] + Pole[i+k-1][j+k-1];
levy = Suma[i+k-1][j+k/2-1] - Suma[i+k-1][j-1] - Suma[i-1][j+k/2-1] + Suma[i-1][j-1];
levy -= Pole[i][j] + Pole[i+k-1][j];
pravy = Suma[i+k-1][j+k-1] - Suma[i+k-1][j+k/2] - Suma[i-1][j+k-1] + Suma[i-1][j+k/2];
pravy -= Pole[i][j+k-1] + Pole[i+k-1][j+k-1];
					}
					printf("\tpro [%d,%d] je %d:%d a %d:%d\n", i, j, horni, dolni, levy, pravy);
					if(horni == dolni && pravy == levy)
						ans = k;
				}
			}
		}
*/