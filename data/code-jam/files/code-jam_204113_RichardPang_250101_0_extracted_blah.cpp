#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <math.h>
#include <iomanip>
#include <numeric>
#include <stdlib.h>
#include <algorithm>
using namespace std;

//ifstream fin("input.txt"); 
ifstream fin("D-small-attempt1.in");
ofstream fout("Dsmall.txt");

int x[50], y[50], r[50];

int sqr(int k) { return k * k; }

double dist(double dx, double dy) {
	return sqrt(dx * dx + dy * dy);
}

double solve(int a, int b) {
	double xx = 0.5 * (x[a] + x[b]);
	double yy = 0.5 * (y[a] + y[b]);
	double rr = 0.5 * (r[a] + r[b] + dist(x[a] - x[b], y[a] - y[b]));

	double d = sqrt(1.0 * sqr(xx - x[3 - a - b]) + sqr(yy - y[3 - a - b]));
	if (d < rr + r[3 - a - b])
		return 1e9;
	return max(1.0 * r[3 - a - b], rr);
}

int main() {
	int t; fin >> t;
	for (int tt = 0; tt < t; tt++) {
		int N; fin >> N;
		for (int i = 0; i < N; i++)
			fin >> x[i] >> y[i] >> r[i];

		double res = 1e9;
		if (N == 1)
			res = r[0];
		else if (N == 2)
			res = min(1.0 * max(r[0], r[1]), 0.5 * (dist(x[0] - x[1], y[0] - y[1]) + r[0] + r[1]));
		else {
			for (int i = 0; i < 2; i++)
				for (int j = 0; j < i; j++)
					res = min(res, solve(i, j));
			double xx = r[0] * x[0] + r[1] * x[1] + r[2] * x[2];
			double yy = r[0] * y[0] + r[1] * y[1] + r[2] * y[2];
			xx /= (r[0] + r[1] + r[2]); yy /= (r[0] + r[1] + r[2]);
			
			double ans = 1e9;
			for (int i = 0; i < 3; i++)
				ans = min(ans, dist(xx - x[i], yy - y[i]) + r[i]);
			res = min(res, ans);
		}
		fout << "Case #" << tt + 1 << ": " << res << endl;
	}
	return 0;
}
