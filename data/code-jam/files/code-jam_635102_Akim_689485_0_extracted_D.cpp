#include <cstdio>
#include <algorithm>
#include <cmath>
using namespace std;
#include <vector>

struct Point {
	double x, y;
	Point(double _x = 0, double _y = 0) {
		x = _x;
		y = _y;
	}
	Point(Point A, Point B) {
		x = B.x - A.x;
		y = B.y - A.y;
	}
};

double VP(Point A, Point B) {
	return A.x * B.y - A.y * B.x;
}

double len(Point A) {
	return sqrt(A.x * A.x + A.y * A.y);
}

double SP(Point A, Point B) {
	return A.x * B.x + A.y * B.y;
}

Point A, B, C, D;



double check() {
	Point v1 = Point(A, B);
	Point v2 = Point(A, C);
	double alpha = acos(SP(v1, v2) / len(v1) / len(v2));
	double ra = len(v2);
	double S1 = alpha * ra * ra;
	v1 = Point(B, A);
	v2 = Point(B, C);
	double beta = acos(SP(v1, v2) / len(v1) / len(v2));
	double rb = len(v2);
	double S2 = beta * rb * rb;
	double S_2ABC = abs(VP(v1, v2));

	double ans = S1 + S2 - S_2ABC;
	if (ans < 0) ans = 0;


//	printf("\n\n%.2lf %.2lf\n", alpha / M_PI * 180, beta / M_PI * 180);
//	printf("%.10lf %.10lf %.10lf   %.10lf\n", S1, S2, S_2ABC, S1 + S2);
//	printf("Ans: %.10lf\n", ans);
	return ans;
	
}

void solve() {
	int n, m;
	scanf("%d%d", &n, &m);
	scanf("%lf%lf%lf%lf", &A.x, &A.y, &B.x, &B.y);
	for (int i = 0; i < m; ++i) {
		scanf("%lf%lf", &C.x, &C.y);
		printf("%.10lf ", check());
	}
	printf("\n");
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
	int T;
	scanf("%d", &T);
	for (int i = 1; i <= T; ++i) {
		printf("Case #%d: ", i);
		solve();
	}
}
