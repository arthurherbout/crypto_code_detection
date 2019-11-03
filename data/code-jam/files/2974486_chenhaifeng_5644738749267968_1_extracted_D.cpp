#include <stdio.h>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

void war(const vector<double> & a, const vector<double> & b, const int & n, int & warOpt) {
	vector<int> f(n, 0);
	for (int i = 0; i < n; i++) {
		double min = 5;
		int mink = -1;
		for (int j = 0; j < n; j++) {
			if (f[j] == 0 && b[j] > a[i]) {
				if (b[j] - a[i] < min) {
					min = b[j] - a[i];
					mink = j;
				}

			}
		}
		if (mink == -1) {
			for (int j = 0; j < n; j++) {
				if (f[j] == 0) {
					f[j] = 1;
					break;
				}
			}
		} else {
			f[mink] = 1;
			warOpt++;
		}
	}
}

void Deceitfulwar(const vector<double> & a, const vector<double> & b, const int & n, int & DeceitfulwarOpt) {
	vector<int> f(n, 0);
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (f[j] == 0) {
				if (a[i] > b[j]) {
					DeceitfulwarOpt++;
					f[j] = 1;
				} else {
					for (int k = n - 1; k >= 0; k--) {
						if (f[k] == 0){
							f[k] = 1;
							break;
						}
					}
				}
				break;
			}
		}
	}
}

int main() {
	//freopen("D-large.in", "r", stdin);
	//freopen("D-large.out", "w", stdout);
	int ca;
	int n;
	double tmp;
	scanf("%d", &ca);
	for (int t = 0; t < ca; t++) {
		printf("Case #%d: ", t + 1);
		scanf("%d", &n);
		vector<double> a, b;
		for (int i = 0; i < n; i++) {
			scanf("%lf", &tmp);
			a.push_back(tmp);
		}

		for (int i = 0; i < n; i++) {
			scanf("%lf", &tmp);
			b.push_back(tmp);
		}
		int warOpt = 0;
		int DeceitfulwarOpt = 0;

		sort(a.begin(), a.end());
		sort(b.begin(), b.end());

		Deceitfulwar(a, b, n, DeceitfulwarOpt);
		war(a, b, n, warOpt);
		printf("%d %d\n", DeceitfulwarOpt, n - warOpt);

	}

	return 0;
}
