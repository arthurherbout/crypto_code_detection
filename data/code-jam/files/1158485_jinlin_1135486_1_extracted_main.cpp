#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

using namespace std;

const double eps = 1e-7;

struct Point {
	double x, y;
} ls[100], us[100];

int T, W, L, U, G;
double totArea;

double getArea(double X) {
	int i;
	double area=0;
	for(i=0; i+1<L&&ls[i+1].x<X; ++i)
		area -= (ls[i+1].x-ls[i].x)*(ls[i+1].y+ls[i].y)/2;
	if(i+1<L) {
		double Y = (ls[i].y*(ls[i+1].x-X)+ls[i+1].y*(X-ls[i].x))
			/(ls[i+1].x-ls[i].x);
		area -= (X-ls[i].x)*(Y+ls[i].y)/2;
	}
	for(i=0; i+1<U&&us[i+1].x<X; ++i)
		area += (us[i+1].x-us[i].x)*(us[i+1].y+us[i].y)/2;
	if(i+1<U) {
		double Y = (us[i].y*(us[i+1].x-X)+us[i+1].y*(X-us[i].x))
			/(us[i+1].x-us[i].x);
		area += (X-us[i].x)*(Y+us[i].y)/2;
	}
	return area;
}

double getX(double area) {
	double lb=0, rb=W;
	while(fabs(lb-rb)>eps) {
		double mb = (lb+rb)/2;
		if(getArea(mb)<area) lb = mb;
		else rb = mb;
	}
	return (lb+rb)/2;
}

int main() {
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		scanf("%d%d%d%d", &W, &L, &U, &G);
		for(int i=0; i<L; ++i)
			scanf("%lf%lf", &ls[i].x, &ls[i].y);
		for(int i=0; i<U; ++i)
			scanf("%lf%lf", &us[i].x, &us[i].y);
		totArea = getArea(W);
		printf("Case #%d:\n", cas);
		for(int i=1; i<G; ++i)
			printf("%.7lf\n", getX(totArea/G*i));
	}
}
