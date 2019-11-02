#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>

using namespace std;

double R,t,f,g,r;

double Sd(double x1, double y1, double x2, double y2)
{
	double sc = 1.0/2.0 * fabs(x1*y2 - x2*y1);
	double fi = asin(2*sc/(R-t-f)/(R-t-f));
	double ssec = fi/2.0*(R-t-f)*(R-t-f);
	return ssec - sc;
}

double ff(double x)
{
	return sqrt((R-t-f)*(R-t-f) - x*x);
}

double sqr(double x)
{
	return x*x;
}

double S(double x, double y)
{
	double x1,y1,x2,y2;
	if (sqr(x)+sqr(y) >= sqr(R-t)) return 0.0;
	if (sqr(x+g) + sqr(y+g) <= sqr(R-t))
	{
		return sqr(g-2*f);
	}
	if (sqr(x+f)+sqr(y+f) >= sqr(R-t-f)) return 0.0;
	if (sqr(x+g-f)+sqr(y+f) <= sqr(R-t-f) && sqr(y+g-f)+sqr(x+f) <= sqr(R-t-f))
	{
		y1 = y+g-f;
		x1 = ff(y1);
		x2 = x+g-f;
		y2 = ff(x2);
		return (g-2*f)*(x1-x-f) + (g-2*f)*(y2-y-f)-(x1-x-f)*(y2-y-f) + 1.0/2.0*(x2-x1)*(y1-y2) + Sd(x1,y1,x2,y2);
	}
	if (sqr(x+g-f)+sqr(y+f) <= sqr(R-t-f))
	{
		x1 = x+f;
		y1 = ff(x1);
		x2 = x+g-f;
		y2 = ff(x2);
		return (g-2*f)*(y2-y-f) + 1.0/2.0*(g-2*f)*(y1-y2) + Sd(x1,y1,x2,y2);
	}
	if (sqr(y+g-f)+sqr(x+f) <= sqr(R-t-f))
	{
		y1 = y+g-f;
		x1 = ff(y1);
		y2 = y+f;
		x2 = ff(y2);
		return (g-2*f)*(x1-x-f) + 1.0/2.0*(g-2*f)*(x2-x1) + Sd(x1,y1,x2,y2); 
	}

	x1 = x+f;
	y1 = ff(x1);
	y2 = y+f;
	x2 = ff(y2);
	return 1.0/2.0*(y1-y-f)*(x2-x-f) + Sd(x1,y1,x2,y2);
}

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int n,i,j;
	fscanf(fi, "%d", &n);
	for(i = 0; i < n; ++i)
	{
		fscanf(fi, "%lf%lf%lf%lf%lf",&f,&R,&t,&r,&g);
		double s = 2*acos(0.0)*R*R;
		if (2*f < g)
		{
			for(double x = r; x <= R; x += g+2*r)
				for(double y = r; y <= R; y += g+2*r)
					s -= 4*S(x,y);					
		}

		fprintf(fo, "Case #%d: %lf\n", i+1, s/2/acos(0.0)/R/R);
	}

	fclose(fi);
	fclose(fo);
	return 0;
}