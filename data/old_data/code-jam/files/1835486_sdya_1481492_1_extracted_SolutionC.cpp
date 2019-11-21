#include <iostream>
#include <cstdio>
#include <vector>
#include <algorithm>
#include <bitset>
#include <cstring>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <ctime>
#pragma comment (linker, "/STACK:256000000")

using namespace std;

const int maxN = 500;
const int maxS = 2100000;

long long n, m, f;
long long p[maxN], s[maxN];
long long a[maxS];
long long d[maxS];

bool check(long long key, long long i) {
	long long x = key / i;
	long long y = key % i;

	if (x > m) {
		return false;
	}
	long long cost = (i - y) * d[x - 1] + y * d[x];
	return cost + i * f <= m;
}

map < long long, long long > dist;
vector < pair < long long, long long > > dp;

const long long inf = 4000000000000000000LL;

long long mul(long long a, long long b) {
	if (a == 0 || b == 0) {
		return a * b;
	}

	if (a > (inf + b - 1LL) / b) return inf;
	return a * b;
}

long long getCost(long long x) {
	long long res = 0;
	long long left_bound = 0LL;
	for (int i = 0; i < dp.size(); ++i) {
		if (x >= dp[i].first) {
			res += mul(dp[i].first - left_bound + 1LL, dp[i].second);
			if (res >= inf) {
				res = inf;
				break;
			}
			left_bound = dp[i].first + 1;
		} else {
			if (x >= left_bound) {
				res += mul(x - left_bound + 1LL, dp[i].second);
				if (res >= inf) {
					res = inf;
					break;
				}
				left_bound = dp[i].first + 1;
			}
		}
	}
	if (x > dp.back().first) {
		res = inf;
	}
	return res;
}

long long getScore(long long key, long long i) {
	long long x = key / i;
	long long y = key % i;

	long long cost = mul(i - y, getCost(x - 1)) + mul(y, getCost(x));
	if (cost >= inf) {
		cost = inf;
	}
	long long add = mul(i, f);
	cost += add;
	if (cost >= inf) {
		cost = inf;
	}
	return cost;
}

bool check(long long key) {
	long long l = 1, r = 3000000000000000000LL;
	while (r - l > 2) {
		long long k1 = (l + l + r) / 3;
		long long k2 = (l + r + r) / 3;

		long long s1 = getScore(key, k1);
		long long s2 = getScore(key, k2);

		if (s1 > s2) {
			l = k1;
		} else {
			r = k2;
		}
	}

	for (long long i = l; i <= r; ++i) {
		long long cur = getScore(key, i);
		if (cur <= m) {
			return true;
		}
	}
	return false;
}

void solve(int test) {
	dist.clear();
	cin >> m >> f >> n;
	for (int i = 1; i <= n; ++i) {
		cin >> p[i] >> s[i];
		if (dist.count(s[i])) {
			dist[s[i]] = min(dist[s[i]], p[i]);
		} else {
			dist[s[i]] = p[i];
		}
	}

	dp.clear();
	for (map < long long, long long > :: iterator it = dist.begin(); it != dist.end(); ++it) {
		dp.push_back(make_pair(it->first, it->second));
	}
	for (int i = (int)(dp.size()) - 2; i >= 0; --i) {
		dp[i].second = min(dp[i].second, dp[i + 1].second);
	}

	long long l = 0, r = 3000000000000000000LL;
	while (r - l > 1LL) {
		long long key = (l + r) / 2LL;

		if (check(key)) {
			l = key;
		} else {
			r = key;
		}
	}

	long long res = 0;
	if (check(r)) {
		res = r;
	} else {
		if (check(l)) {
			res = l;
		}
	}

	printf("Case #%d: %lld\n", test, res);
}

/*void solve(int test) {
	cin >> m >> f >> n;
	for (int i = 0; i < maxS; ++i) {
		a[i] = 1000000000LL;
	}
	for (int i = 1; i <= n; ++i) {
		cin >> p[i] >> s[i];
		a[s[i]] = min(a[s[i]], p[i]);
	}
	for (int i = 0; i <= m; ++i) {
		d[i] = -1;
	}

	for (int i = maxS - 2; i >= 0; --i) {
		a[i] = min(a[i + 1], a[i]);
	}

	d[0] = a[0];
	for (int i = 1; i < maxS; ++i) {
		d[i] = d[i - 1] + a[i];
		if (d[i] > m) {
			d[i] = m + 1; 
		}
	}

	long long res = 0;
	for (long long i = 1; i < maxS; ++i) {
		if ((long long)(i) * (long long)(f) > m) {
			break;
		}

		long long l = i, r = 1000000000LL;
		while (r - l > 1LL) {
			long long key = (l + r) / 2;

			if (check(key, i)) {
				l = key;
			} else {
				r = key;
			}
		}
		if (check(l, i)) {
			res = max(res, l);
		}
		if (check(r, i)) {
			res = max(res, r);
		}
	}
	printf("Case #%d: %lld\n", test, res);
}*/

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	int tests;
	scanf("%d", &tests);
	for (int i = 0; i < tests; ++i) {
		solve(i + 1);
		cerr << i << endl;
	}
	return 0;
}