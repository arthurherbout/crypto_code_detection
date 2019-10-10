#include <stdio.h>

int casenum;
int n,m;
int query[110][11][2];
int result, bitresult;
int getcount(int v)
{
	int num =0;
	while (v)
	{
		num+=v &0x01;
		v >>=1;
	}
	return num;
}
int issati(int index, int nbit, int res)
{
	index >>=(nbit-1);
	if ((index &0x01) == res) return 1;
	else return 0;
}
int check(int index)
{
	int i,j,k,sati;
	for (i = 0; i < m; i ++)
	{
		sati = 0;
		for (j = 1; j <= query[i][0][0]; j ++)
			if (issati(index, query[i][j][0], query[i][j][1])) 
			{
				sati = 1;
				break;
			}
		if (!sati) return 0;
	}
	return 1;
}
int main()
{
	int i,j,maxnum, isfind,tmp;
	freopen("B-small-attempt0.in","r",stdin);
	freopen("bsout.out", "w",stdout);
	scanf("%d",&casenum);
	for (int kase = 1;  kase <= casenum; kase++)
	{
		scanf("%d %d",&n, &m);
		for (i = 0; i < m; i ++)
		{
			scanf("%d", &query[i][0][0]);
			for (j = 1; j <= query[i][0][0]; j ++)
				scanf("%d %d", &query[i][j][0],&query[i][j][1]);
		}
		maxnum = 1;
		for (i = 0; i < n; i ++)
			maxnum *=2;
		isfind = 0;

		for (i =0; i < maxnum; i ++)
		{
			if (check(i))
			{
				if (!isfind)
				{
					isfind = 1;
					result = i;
					bitresult = getcount(result);
				}
				else 
				{
					int tmp = getcount(i);
					if (tmp < bitresult)
					{
						bitresult = tmp;
						result = i;
					}
				}
			}
		}
		if (!isfind) printf("Case #%d: IMPOSSIBLE\n",kase);
		else 
		{
			printf("Case #%d:",kase);
			tmp = result;
			for (i =0; i < n; i ++)
			{
				printf(" %d", tmp &0x01);
				tmp>>=1;
			}
			printf("\n");
		}
	}
	return 0;
}
