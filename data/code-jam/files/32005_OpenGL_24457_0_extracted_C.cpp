#include <cstdio>
#include <cstring>
#include <ctime>

int	M , X;
double	P;

bool	calced	[16][1000000];
double	opt	[16][1000000];

#define	prob(bet) ((dp(m-1,x+bet)*P)+dp(m-1,x-bet)*(1-P))

double	dp(int m, int x)
{
	if (x >= 1000000) return 1;
	if (m == 0) return 0;
	if (calced[m][x]) return opt[m][x];
	if ((long long)x * (1<<m) < 1000000) return 0;
	calced[m][x] = 1;
	double	& ret = opt[m][x];
	double	tmp;
	/*
	for (int bet = 1; bet <= x; bet ++)
	{
		tmp = dp(m - 1, x + bet) * P
			+ dp( m - 1 , x - bet ) * (1 - P);
		if (tmp > ret) ret = tmp;
	}
	*/
	int	low = 1;
	int	upp = x;
	int	m1 , m2;
	double	v1 , v2;
	while (upp - low >= 3)
	{
		m1 = low + (upp - low) / 3;
		m2 = m1  + (upp - low) / 3;
		v1 = prob(m1);
		v2 = prob(m2);
		if (v1 > v2) upp = m2;
		else low = m1;
	}
	for (; low <= upp; low ++)
	{
		tmp = prob(low);
		if (tmp > ret) ret = tmp;
	}
	return	ret;
}

void	solve()
{
	memset(calced , 0 , sizeof(calced));

	printf("%.6lf\n" , dp(M,X));
}

int	main()
{
//	freopen("in.txt" , "r" , stdin);
//	freopen("C-small-attempt0.in" , "r" , stdin); freopen("C-small-attempt0.out" , "w" , stdout);
	freopen("C-small-attempt1.in" , "r" , stdin); freopen("C-small-attempt1.out" , "w" , stdout);
//	freopen("C-large.in", "r" , stdin); freopen("C-large.out" , "w" , stdout);

	int	caseNo , t;
	scanf("%d" , &caseNo);
	for (t = 1; t <= caseNo; t ++)
	{
		fprintf(stderr,  "%d\n" , t);
		scanf("%d%lf%d" , &M , &P , &X);
		printf("Case #%d: " , t);
		solve();
	}

	fprintf(stderr , "%.3lfs\n" , clock() / (double)CLK_TCK);

	return 0;
}
