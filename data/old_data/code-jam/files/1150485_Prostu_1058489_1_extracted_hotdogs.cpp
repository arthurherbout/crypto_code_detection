#include <fstream>
#include <iomanip>
#include <cstring>

using namespace std;

const int NMAX = 256;

const double eps = 1e-4;
const double VMAX = 1e+6;

int V[NMAX], P[NMAX];
int C, D;

bool works(double time) {
	int i, j;
	double last, current;

	last = P[0] - time;

	for (i = 0; i < C; ++i) {
		for (j = 0; j < V[i]; ++j) {
			if (last + D < P[i] - time)
				last = P[i] - time;
			else {
				current = max(last + D, P[i] - time);
				if (current > P[i] + time) return false;
				last = current;
			}
		}
	}

	return true;
}

int main(void) {

	ifstream fin("input.txt");
	FILE *fout = fopen("output.txt", "wt");
	int T, ncase;
	int i, N;
	double start, mid, end;

	fin >> T;

	for (ncase = 1; ncase <= T; ++ncase) {
		printf("%d\n", ncase);
		fin >> C >> D;

		N = 0;
		for (i = 0; i < C; ++i) {
			fin >> P[i] >> V[i];
			N += V[i];
		}
		V[0]--;

		start = 0; end = VMAX * N;

		while (start + eps < end) {
			mid = (start + end) / 2;
			if (works(mid)) end = mid;
			else start = mid;
		}

		fprintf(fout, "Case #%d: %.2lf\n", ncase, start);		
		//fout << "Case #" << ncase << ": " << setprecision(4) << start << '\n';
	}

	fin.close();
//	fout.close();

	return 0;
}
