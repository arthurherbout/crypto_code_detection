#include <cstdio>
#include <bitset>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>

using namespace std;

double entrophy(int a[]) {
	double ret = 0;
	vector<int> count(1000, 0);
	for (int i = 0; i < 1000; ++i) {
		int dist = abs(a[i] - i);
		count[dist]++;
	}
	for (int i = 0; i < 1000; ++i) {
		if (count[i] > 0)
			ret -= count[i] / 1000.0 * log2(count[i] / 1000.0);
	}
	return ret;
}

void gao(int a[]) {
	for (int i = 0; i < 1000; ++i) {
		a[i] = i;
	}
	for (int i = 0; i < 1000; ++i) {
		int k = rand() % 1000;
		swap(a[i], a[k]);
	}
}

void learning() {
	int a[1000];
	double e1 = 0;
	double e2 = 0;
	for (int i = 0; i < 10000; ++i) {
		random_shuffle(a, a + 1000);
		e1 += entrophy(a);
	}
	srand(time(NULL));
	for (int i = 0; i < 10000; ++i) {
		gao(a);
		e2 += entrophy(a);
	}
	printf("%lf %lf\n", e1 / 10000, e2 / 10000);
}

int main() {
	// learning();
	int T;
	scanf("%d", &T);
	vector<double> e;
	for (int re = 1; re <= T; ++re) {
		int n;
		scanf("%d", &n);
		vector<int> a(n);
		for (int i = 0; i < n; ++i) {
			scanf("%d", &a[i]);
		}
		e.push_back(entrophy(&a[0]));
	}
	std::vector<double> vd = e;
	sort(vd.begin(), vd.end());

	fprintf(stderr, "%lf\n", vd[vd.size() / 2]);

	for (int i = 0; i < e.size(); ++i) {
		printf("Case #%d: ", i + 1);
		if (e[i] >= vd[vd.size() / 2]) {
			puts("BAD");
		} else {
			puts("GOOD");
		}
	}
	return 0;
}