#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <numeric>
using namespace std;

#define FOR(i,a,b) for(int i=(a); i<(int)(b); ++i)
#define ALL(a) (a).begin(),(a).end()
#define PB(a) push_back(a)
#define MP(a,b) make_pair((a),(b))
#define sqr(a) ((a)*(a))
typedef long long i64;
typedef unsigned long long u64;

int nextInt() {
	int x;
	scanf("%d", &x);
	return x;
}

long long nextLong() {
	long long x;
	scanf("%I64d", &x);
	return x;
}

double nextDouble() {
	double x;
	scanf("%lf", &x);
	return x;
}

const int BUFSIZE = 1000000;
char buf[BUFSIZE + 1];
string nextString() {
	scanf("%s", buf);
	return buf;
}

typedef vector<vector<int> > Adj;

string toBinary(long long x) {
	string res;
	if (x == 0) {
		return "0";
	}
	while (x) {
		res += x % 2 + '0';
		x /= 2;
	}
	reverse(res.begin(), res.end());
	return res;
}

bool isSquare(long long x) {
	long long lo = 0, hi = (1 << 30) + 100;
	while (lo <= hi) {
		long long mid = (lo + hi) / 2;
		if (mid * mid < x) {
			lo = mid + 1;
		} else {
			hi = mid - 1;
		}
	}
	return (lo * lo == x);
}

string brute(string s) {
	vector<long long> add;
	long long have = 0;
	for (int i = 0; i < s.size(); ++i) {
		if (s[i] == '?') {
			add.push_back(1LL << (s.size() - 1 - i));
		} else if (s[i] == '1') {
			have += 1LL << (s.size() - 1 - i);
		}
	}
	for (int mask = 0; mask < (1 << add.size()); ++mask) {
		long long x = have;
		for (int i = 0; i < add.size(); ++i) {
			if (mask & (1 << i)) {
				x += add[i];
			}
		}
		if (isSquare(x)) {
			return toBinary(x);
		}
	}
	return false;
}

int main() {
	freopen("D-small-attempt0.in", "rt", stdin);
	freopen("D-small-attempt0.out", "wt", stdout);
	int T = nextInt();
	for (int cas = 1; cas <= T; ++cas) {
		string s = nextString();
		string res = brute(s);
		cerr << cas << endl;
		printf("Case #%d: %s\n", cas, res.c_str());
	}
	return 0;
}