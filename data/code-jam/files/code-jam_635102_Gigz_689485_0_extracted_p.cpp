#include <cstdio>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <map>
#include <set>
#include <utility>
using namespace std;

#define FOR(i,n) for(i=0; i<(n); ++i)
#define SFOR(i,m,n) for(i=(m); i<(n); ++i)

double A[100];
double PX[100], PY[100], QX[100], QY[100];

int main()
{
	int t, tt;
	FILE *fi = fopen("a.in", "rt");
	FILE *fo = fopen("a.out", "wt");
	fscanf(fi, "%d", &tt);
	for (t = 0; t < tt; ++t)
	{
		int n, m, i, j;
		fscanf(fi, "%d%d", &n, &m);

		FOR(i,n) fscanf(fi, "%lf%lf", &PX[i], &PY[i]);
		FOR(i,m) fscanf(fi, "%lf%lf", &QX[i], &QY[i]);

		double D = sqrt((PX[0]-PX[1])*(PX[0]-PX[1]) + (PY[0]-PY[1])*(PY[0]-PY[1]));

		FOR(i, m)
		{
			double R1 = sqrt((PX[0]-QX[i])*(PX[0]-QX[i]) + (PY[0]-QY[i])*(PY[0]-QY[i]));
			double R2 = sqrt((PX[1]-QX[i])*(PX[1]-QX[i]) + (PY[1]-QY[i])*(PY[1]-QY[i]));
			double fi1 = acos((D*D + R1*R1 - R2*R2)/2/D/R1);
			double fi2 = acos((D*D + R2*R2 - R1*R1)/2/D/R2);

			cout<<D<<endl<<R1<<endl<<R2<<endl<<endl;
			cout<<fi1<<endl<<fi2<<endl<<endl;

			A[i] = fi1*R1*R1 - R1*R1*cos(fi1)*sin(fi1) + fi2*R2*R2 - R2*R2*cos(fi2)*sin(fi2);
		}

		fprintf(fo, "Case #%d:", t+1);
		FOR(i, m) fprintf(fo, " %lf", A[i]);
		fprintf(fo, "\n");
	}
	fclose(fi);
	fclose(fo);
	return 0;
}