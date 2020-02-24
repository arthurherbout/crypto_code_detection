#include <iostream>
#include <cstdio>

using namespace std;

const double eps = 1e-5;

int n;
double X;
double s[200];

double find(int k, double l, double r) {
	if(r-l<eps) return (r+l)/2;
	double mid = (r+l)/2/100;
	double cur = mid*X+s[k];
	double other = 0.0;
	for(int i=0; i<n; ++i) {
		if(i==k) continue;
		other += (cur>s[i]?cur-s[i]:0.0);
	}
	if(other/X+mid<1.0) return find(k, (l+r)/2, r);
	return find(k, l, (l+r)/2);
}

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		scanf("%d", &n);
		X = 0;
		for(int i=0; i<n; ++i) {
			scanf("%lf", s+i);
			X += s[i];
		}
		printf("Case #%d:", cas);
		for(int i=0; i<n; ++i)
			printf(" %.6lf", find(i,0.0,100.0));
		printf("\n");
	}
}
