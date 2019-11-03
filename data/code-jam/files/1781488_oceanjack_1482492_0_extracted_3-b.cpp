#include<stdio.h>
#include<math.h>
int main()
{
	const int mmax = 2012;
	double gao[mmax], fuck[mmax], ca;
	int c, sc = 1;
	freopen("in.in","r",stdin);
	freopen("out.txt","w",stdout);
	for(scanf("%d",&c); c; --c, ++sc)
	{
		int jj, kk;
		double tmp, tmp2;
		double ii;
		printf("Case #%d:\n",sc);
		scanf("%lf%d%d",&ii, &jj, &kk);
		for(int i = 0; i < jj; ++i)
			scanf("%lf%lf",&gao[i],&fuck[i]);
		for(int i = 0; i < kk; ++i)
		{
			scanf("%lf",&ca);
			if(fuck[0] > ii)
			{
				tmp = sqrt(2.0 * ii / ca);
				printf("%lf\n",tmp);
			}
			else
			{
				double v = (double)((fuck[1] - fuck[0]) / (gao[1] - gao[0]));
				tmp2 = (ii-fuck[0]) / v;
				tmp = sqrt(2.0*ii/ca);
				(tmp > tmp2) ? printf("%lf\n",tmp) : printf("%lf\n",tmp2);
			}
		}
	}
	return 0;
}
