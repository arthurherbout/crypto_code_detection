#include <iostream>
#include <cstdio>
using namespace std;

int T, N;
double V, X, R[107], C[107];
const double eps = 1E-10;

int main() {
	cin >> T;
	for (int t=1; t<=T; t++) {
		cin >> N >> V >> X;
		for (int i=0; i<N; i++)
			cin >> R[i] >> C[i];

		cout << "Case #" << t << ": ";
		if (N == 1 && fabs(X-C[0]) > eps) {
			cout << "IMPOSSIBLE" << endl;
		} else if (N == 1) {
			printf("%.7f\n", V/R[0]);
		} else if (N == 2 && (C[0] > X && C[1] > X || C[0] < X && C[1] < X)) {
			cout << "IMPOSSIBLE" << endl;
		} else if (N == 2 && fabs(C[0]-X) < eps && fabs(C[1]-X) < eps) {
			printf("%.7f\n", V/(R[0]+R[1]));
		} else if (N == 2 && fabs(C[0]-X) < eps) {
			printf("%.7f\n", V/R[0]);
		} else if (N == 2 && fabs(C[1]-X) < eps) {
			printf("%.7f\n", V/R[1]);
		} else {
			double v = V/(C[0]-C[1])*(X-C[1]);
			printf("%.7f\n", max(v/R[0], (V-v)/R[1]));
		}
	}
}
