#define _CRT_SECURE_NO_DEPRECATE
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
using namespace std;

typedef pair<int, int> pii;
vector<pii> points;
int result;

inline pii rotate(pii p1, pii p2) {
	int x = p1.first - p2.first;
	int y = p1.second - p2.second;
	return pii(p2.first + y, p2.second - x);
}

int n, m;
inline int sqr(int p) {
	return p*p;
}

void perebor(int k, pii cur) {
	int cl = cur.first*cur.first + cur.second*cur.second;
	result = max(result, cl);
	int maxPossible = sqr(400*k + max(abs(cur.first), abs(cur.second))) + sqr(min(abs(cur.first), abs(cur.second)));
	if (maxPossible < result)
		return;
	if (k != 0) {
		for (size_t i = 0; i < points.size(); ++i) {
			perebor(k - 1, rotate(cur, points[i]));
		}
	}
}

int main(void) {
	int testCount;
	scanf("%d", &testCount);
	for (int testNo = 1; testNo <= testCount; ++testNo) {
		scanf("%d%d", &n, &m);
		result = 0;
		points.resize(n);
		for (int i = 0; i < n; ++i) {
			scanf("%d%d", &points[i].first, &points[i].second);
		}
		perebor(m, pii());
		printf("Case #%d: %.9lf\n", testNo, sqrt(double(result)));
	}
	return 0;
}
