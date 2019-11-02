#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <algorithm>
#include <deque>
#include <cmath>
#include <ctime>

#pragma comment (linker, "/STACK:64000000")

using namespace std;

struct ppp {
	double alive;
	double len;
	int id;

	bool operator > (const ppp& ot) const {
		if (fabs(alive - ot.alive) > 1e-9)
			return alive < ot.alive;
		return id < ot.id;
	}

};

ppp s[11111];

double calc(const ppp& a, const ppp& b) {
	return (a.len + b.len * a.alive)/(a.alive * b.alive);
}

bool cmp2(const ppp& a, const ppp& b) {
	double ans1 = calc(a,b);
	double ans2 = calc(b,a);

	if (fabs(ans1 - ans2) < 1e-8) return a.id < b.id;

	return ans1 < ans2;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int t; cin >> t;

	for (int e=1; e<=t; e++) {

		int n; cin >> n;
		for (int i=0; i<n; i++) {
			cin >> s[i].len;
			s[i].id = i;
		}

		for (int i=0; i<n; i++) {
			cin >> s[i].alive;
			s[i].alive /= 100;
			s[i].alive = 1.0 - s[i].alive;
		}

		sort(s, s+n, cmp2);

		cout << "Case #" << e << ":";

		for (int i=0; i<n; i++)
			cout << ' ' << s[i].id;

		cout << endl;
	}

	return 0;
}