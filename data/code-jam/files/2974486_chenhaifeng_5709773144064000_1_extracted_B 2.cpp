#include <stdio.h>
#include <iostream>
using namespace std;

int main() {
	//freopen("B-large.in", "r", stdin);
	//freopen("B-large.out", "w", stdout);
	int t;
	double c, f, x;
	scanf("%d", &t);
	for (int i = 0; i < t; i++) {
		scanf("%lf%lf%lf", &c, &f, &x);
		//printf("%lf %lf %lf\n", c, f, x);
		double time = 0, t2, t3;
		double speed = 2;
		//int nFarm = 0;
		while (1) {
			t2 = x / speed;
			t3 = c / speed + (x / (speed + f));
			if (t2 < t3) {
				time = time + t2;
				break;
			}
			time += c / speed;
			speed += f;
		}
		printf("Case #%d: %.7lf\n", i + 1, time);
	}

	return 0;
}
