#include<stdio.h>
#include<math.h>


void solve()
{
	char match[200][200];
	int n;
	scanf("%d", &n);
	for(int i = 0; i < n; i++)
		scanf("%s", &match[i]);

	long double WP[200], OWP[200], OOWP[200];

	for(int i = 0; i < n; i++)
	{
		int count = 0;
		double sum = 0;
		for(int j = 0; j < n; j++)
		{
			if(match[i][j] != '.' )
			{
				sum += (match[i][j] - '0');
				count++;
			}
		}
		WP[i] = sum/count;
	}

	for(int i = 0; i < n; i++)
	{
		OWP[i] = 0;
		int tcount = 0;
		for(int j = 0; j < n; j++)
		{
			int count = 0;
			double sum = 0;

			if(match[j][i] != '.')
			{
				tcount++;
				for(int k = 0; k < n; k++)
				{
					if(k == i || match[j][k] == '.') continue;
					count++;
					sum += (match[j][k] - '0');
				}
				sum = sum/count;
				OWP[i] += sum;
			}
		}
		OWP[i] /= tcount;
	}


	for(int i = 0; i < n; i++)
	{
		OOWP[i] = 0;
		int count = 0;
		for(int j = 0; j < n; j++)
		{
			if(match[j][i] != '.')
			{
				count++;
				OOWP[i] += OWP[j];
			}

		}
		OOWP[i]/= count;
	}

	for(int i = 0; i < n; i++)
		printf("%.10lf\n", 0.25 * WP[i] + 0.50 * OWP[i] + 0.25 * OOWP[i]);
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("data.txt","r",stdin);
	freopen("out.txt","w",stdout);
#endif

	int T;
	scanf("%d\n", &T);
	for(int i = 1; i <= T; i++)
	{
		printf("Case #%d:\n", i);
		solve();
	}

	return 0;

}
