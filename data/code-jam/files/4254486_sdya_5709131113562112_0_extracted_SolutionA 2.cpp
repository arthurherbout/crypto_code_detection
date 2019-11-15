#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
#include <string>
#include <queue>
#include <set>
#include <map>
#include <bitset>
#include <cmath>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 1100000;
int n, d;
long long s[maxN], m[maxN];
long long as, cs, rs;
long long am, cm, rm;

long long p[maxN], q[maxN];
long long l[maxN], r[maxN];

int t[16 * maxN];
int add[16 * maxN];

void init(int i, int l, int r) {
	if (l == r) {
		t[i] = add[i] = 0;
		return ;
	}
	init(2 * i, l, (l + r) / 2);
	init(2 * i + 1, (l + r) / 2 + 1, r);
	t[i] = add[i] = 0;
}

void update(int i, int l, int r, int cl, int cr, int delta) {
	if (l == cl && r == cr) {
		t[i] += delta;
		add[i] += delta;
		return ;
	}
	if (cl > (l + r) / 2) {
		update(2 * i + 1, (l + r) / 2 + 1, r, cl, cr, delta);
	} else if (cr <= (l + r) / 2) {
		update(2 * i, l, (l + r) / 2, cl, cr, delta);
	} else {
		update(2 * i, l, (l + r) / 2, cl, (l + r) / 2, delta);
		update(2 * i + 1, (l + r) / 2 + 1, r, (l + r) / 2 + 1, cr, delta);
	}
	t[i] = max(t[2 * i], t[2 * i + 1]) + add[i];
}

int get(int i, int l, int r, int cl, int cr) {
	if (l == cl && r == cr) {
		return t[i];
	}
	if (cl > (l + r) / 2) {
		return add[i] + get(2 * i + 1, (l + r) / 2 + 1, r, cl, cr);
	}
	if (cr <= (l + r) / 2) {
		return add[i] + get(2 * i, l, (l + r) / 2, cl, cr);
	}
	return add[i] + max(get(2 * i, l, (l + r) / 2, cl, (l + r) / 2), get(2 * i + 1, (l + r) / 2 + 1, r, (l + r) / 2 + 1, cr));
}

void readData() {
}

void solve(int test) {
	cin >> n >> d;
	cin >> s[0] >> as >> cs >> rs;
	cin >> m[0] >> am >> cm >> rm;
	for (int i = 0; i + 1 < n; ++i) {
		s[i + 1] = (s[i] * as + cs) % rs;
		m[i + 1] = (m[i] * am + cm) % rm;
	}
	for (int i = 1; i < n; ++i) {
		m[i] %= i;
	}

	p[0] = s[0];
	q[0] = s[0];
	for (int i = 1; i < n; ++i) {
		p[i] = min(s[i], p[m[i]]);
		q[i] = max(s[i], q[m[i]]);
	}

	l[0] = s[0] - d;
	r[0] = s[0];
	for (int i = 1; i < n; ++i) {
		l[i] = q[i] - d;
		r[i] = p[i];
	}
	vector < long long > a;
	for (int i = 0; i < n; ++i) {
		a.push_back(l[i]);
		a.push_back(r[i]);
	}
	sort(a.begin(), a.end());
	a.resize(unique(a.begin(), a.end()) - a.begin());

	init(1, 0, (int)(a.size()) - 1);
	for (int i = 0; i < n; ++i) {
		if (l[i] <= r[i]) {
			int x = lower_bound(a.begin(), a.end(), l[i]) - a.begin();
			int y = lower_bound(a.begin(), a.end(), r[i]) - a.begin();
			update(1, 0, (int)(a.size()) - 1, x, y, +1);
		}
	}
	int L = lower_bound(a.begin(), a.end(), l[0]) - a.begin();
	int R = lower_bound(a.begin(), a.end(), r[0]) - a.begin();
	printf("Case #%d: %d\n", test, get(1, 0, (int)(a.size()) - 1, L, R));
}

int main(int argc, char* argv[]) {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int left_bound, right_bound;

	/*freopen(argv[3], "w", stdout);	
	sscanf(argv[1], "%d", &left_bound);
	sscanf(argv[2], "%d", &right_bound);*/

	int t;
	cin >> t;
	left_bound = 1, right_bound = t;
	for (int i = 1; i <= t; ++i) {
		if (i >= left_bound && i <= right_bound) {
			solve(i);
		}
		cerr << i << ": " << clock() << endl;
	}

	return 0;
}
