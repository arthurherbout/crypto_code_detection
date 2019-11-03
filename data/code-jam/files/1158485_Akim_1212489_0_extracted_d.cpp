#include <cstdio>
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <queue>
#define pb push_back
#define mp make_pair
#define sz(v) ((int)(v).size())
#define all(v) (v).begin(),(v).end()

using namespace std;

typedef pair<int, int> ii;
typedef long long int64;
typedef vector<int> vi;

template<class T> T abs(T a) {return a > 0 ? a : (-a); }
template<class T> T sqr(T a) {return a * a; }

using namespace std;

bool cool(long long n) {
//	cerr << n << endl;
	long long L = 0;
	long long R = 1;
	while (R * R < n) R *= 2;
	while (L != R - 1) {
		long long tmp = (L + R) / 2;
		if (tmp * tmp > n) R = tmp; else L = tmp;
		if (tmp * tmp == n) return true;
	}
	return (L * L == n || R * R == n);
}

void solve(int testnum) {
	cerr << testnum << endl;
	string n;
	cin >> n;
	int q = 0;
	for (int i = 0; i < sz(n); ++i) if (n[i] == '?') ++q;
	string ans;
	for (int mask = 0; mask < (1 << q); ++mask) {
//		if (mask % 100000 == 0) cerr << mask << " " << (1 << q) << endl;
		string tmp;
		tmp = n;
		int cur = 0;
		for (int i = 0; i < sz(n); ++i)
			if (n[i] == '?') {
				if (mask & (1 << cur)) tmp[i] = '1'; else tmp[i] = '0';
				++cur;
			}
		long long num = 0;
		for (int i = 0; i < sz(n); ++i) {
			num *= 2;
			num += tmp[i] - '0';
		}
//		cerr << tmp << endl;
		if (cool(num)) {
			ans = tmp;
			break;
		}
	}
	cout << "Case #" << testnum << ": " << ans << endl;
}

int main() {
	int t;
	scanf("%d", &t);
	for (int i = 1; i <= t; ++i) solve(i);
	return 0;
}
