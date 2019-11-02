#include <fstream>
#include <iomanip>
#include <iostream>
#include <math.h>
using namespace std;

double f, R, t, r, g;

inline double sqr(double k) { return k * k; }
double pi = 3.1415926535898;

double find(double x1, double x2, double y) {
	double res = 0;
	if (y + g < R) {
		double xx = sqrt(sqr(R) - sqr(y + g));
		if (xx > x1) {
			res += g * (xx - x1);
			x1 = xx;
		}
	}
	x2 = min(x2, sqrt(sqr(R) - sqr(y)));

	res += x2 * sqrt(sqr(R) - sqr(x2)) / 2 - x1 * sqrt(sqr(R) - sqr(x1)) / 2;
	res += sqr(R) / 2 * (asin(x2 / R) - asin(x1 / R));
	res -= y * (x2 - x1);
	return res;
}

int main() {
	ifstream fin("C-small-attempt0.in");
	ofstream fout("C-small.out");
	int n;
	fin >> n;
	for (int c = 0; c < n; c++) {
		fin >> f >> R >> t >> r >> g;
		double total = pi * sqr(R);

		t += f;
		g -= 2 * f;
		r += f;
		R -= t;

		double area = 0;
		for (double x1 = r; x1 < R; x1 += g + 2 * r) {
			for (double y1 = r; y1 < R; y1 += g + 2 * r) {
				if (sqr(x1) + sqr(y1) > sqr(R))
					y1 = R + 1;
				else if (sqr(x1 + g) + sqr(y1 + g) < sqr(R))
					area += sqr(g);
				else
					area += find(x1, x1 + g, y1);
			}
		}
		double res = 1 - (4 * area / total);
		fout.setf(ios::fixed);
		fout << "Case #" << c + 1 << ": " << setprecision(6) << res << endl;
	}
	return 0;
}
