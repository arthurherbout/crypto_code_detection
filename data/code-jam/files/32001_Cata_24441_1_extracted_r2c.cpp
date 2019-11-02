#include <cstdio>
#include <vector>
using namespace std;

double abs(double x)
{
	return (x<0)?(-x):x;
}

int main()
{
	int n, T;
	vector<int> x(1000);	
	vector<int> y(1000);
	vector<int> z(1000);
	vector<int> p(1000);
	int i, j;
	int t;
	scanf ("%d", &T);
	for (t=1; t<=T; t++)
	{
		scanf("%d", &n);
		for (i=0; i<n; i++)
			scanf("%d %d %d %d", &x[i], &y[i], &z[i], &p[i]);
		double max=0;
		for (i=0; i<n; i++)
			for (j=i+1; j<n; j++)
		{
			double d=abs(x[i]-x[j])+abs(y[i]-y[j])+abs(z[i]-z[j]);
			d/=(p[i]+p[j]);
			if (max<d)
				max=d;
		}
		printf("Case #%d: %.6f\n", t, max);
	}
	return 0;
}
