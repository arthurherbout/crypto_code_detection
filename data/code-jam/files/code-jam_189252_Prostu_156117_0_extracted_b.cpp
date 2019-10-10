using namespace std;

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define FORI(p, X) for (__typeof( (X).begin() ) p = (X).begin(); p != (X).end(); ++p)
#define ALL(X) (X).begin(), (X).end()
#define PB push_back
#define MP make_pair

const int INF = 0x3f3f3f3f;

typedef pair <int, int> PII;
typedef vector <int> VI;
typedef long long lint;

int N;
double X, Y, Z, VX, VY, VZ;

double sqr(double x) {
	return x * x;
}

double f(double t) {
	return sqrt( sqr((X + t * VX) / N)  + sqr((Y + t * VY) / N) + sqr((Z + t * VZ) / N) );
}

int main(void) {
	FILE *fin = fopen("B-small.in", "rt");
	FILE *fout = fopen("output.out", "wt");
	int ncase, NCASE;
	double x, y, z, vx, vy, vz;
	double mij1, mij2, beg, end;
	int i;

	fscanf(fin, " %d", &NCASE);

	for (ncase = 1; ncase <= NCASE; ++ncase) {

		fscanf(fin, " %d", &N);

		X = Y = Z = VX = VY = VZ = 0;
		for (i = 0; i < N; ++i) {
			fscanf(fin, " %lf %lf %lf %lf %lf %lf", &x, &y, &z, &vx, &vy, &vz);
			X += x, Y += y, Z += z;
			VX += vx, VY += vy, VZ += vz;
		}

		beg = 0.; end = 1e+4;
		for (i = 0; i < 500; ++i) {
			mij1 = (2 * beg + end) / 3;
			mij2 = (beg + 2 * end) / 3;

			if (f(mij2) < f(mij1))
				beg = mij1;
			else
				end = mij2;
		}

		fprintf(fout, "Case #%d: %.7lf %.7lf\n", ncase, f(beg), beg);
	}


	fclose(fin);
	fclose(fout);

	return 0;
}
