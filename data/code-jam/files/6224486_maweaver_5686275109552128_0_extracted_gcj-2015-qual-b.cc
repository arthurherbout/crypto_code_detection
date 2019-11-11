//#undef _GLIBCXX_DEBUG

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <valarray>
#include <vector>

using namespace std;

#define foreach(iter,cont) for(typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); ++iter)
#define min3(a, b, c) min(a, min(b, c))
#define max3(a, b, c) max(a, max(b, c))
#define inf 1000000008
#define eps 1.0e-10L
#define divisible(a, b) (((a) / (b)) * (b) == (a))
//static const long long p = 1000000007;
__attribute__((unused)) static bool eq(long double a, long double b)  { return (a == b ? true : (a * b == 0 ? abs(a - b) < eps : abs(a - b) / (abs(a) + abs(b)) < eps));  }
__attribute__((unused)) static long long modp(long long n, long long m) { long long r = n % m; return r >= 0 ? r : r + m; }
__attribute__((unused)) static long long gcd(long long a, long long b) { return (a < 0 ? gcd(-a, b) : (b < 0 ? gcd(a, -b) : (!a && !b ? 1 : (b ? gcd(b, a % b) : a)))); }
template<class T> static int len(const T& cont) { return (int) cont.size(); }

// BEGIN CUT HERE
template<class T, class U> ostream &operator<<(ostream &os, const pair<T, U> &p) { return os << "(" << p.first << ", " << p.second << ")"; }
template<class T> ostream &operator<<(ostream& os, const vector<T> &vec) { os << "[ "; for(size_t i = 0; i < vec.size(); i++) { os << vec[i]; if(i != vec.size() - 1) { os << ", "; } } return os << " ]"; }
template<class T> ostream &operator<<(ostream& os, const list<T> &l) { os << "[ "; bool first = true; for(typename list<T>::const_iterator lstIt = l.begin(); lstIt != l.end(); ++lstIt) { if(!first) { os << ", "; } first = false; os << *lstIt; } return os << " ]"; }
template<class T> ostream &operator<<(ostream& os, const deque<T> &d) { os << "[ "; bool first = true; for(typename deque<T>::const_iterator dIt = d.begin(); dIt != d.end(); ++dIt) { if(!first) { os << ", "; } first = false; os << *dIt; } return os << " ]"; }
template<class T> ostream &operator<<(ostream& os, const vector<vector<T> > &mat) { for(size_t i = 0; i < mat.size(); i++) { if(i) os << endl; os << mat[i]; } return os; }
template<class T> ostream &operator<<(ostream &os, const set<T> &s) { os << "{ "; bool first = true; for(typename set<T>::const_iterator setIt = s.begin(); setIt != s.end(); ++setIt) { if(!first) { os << ", "; } first = false; os << *setIt; } return os << " }"; }
template<class T, class U> ostream &operator<<(ostream &os, const map<T, U> &m) { os << "{ "; bool first = true; for(typename map<T, U>::const_iterator mapIt = m.begin(); mapIt != m.end(); ++mapIt) { if(!first) { os << ", "; } first = false; os << mapIt->first << " = " << mapIt->second; } return os << " }"; }
enum CONSOLE_FG { FG_BLACK, FG_RED, FG_GREEN, FG_YELLOW, FG_BLUE, FG_MAGENTA, FG_CYAN, FG_WHITE };
enum CONSOLE_STYLE { STYLE_NONE, STYLE_BOLD };
ostream& operator<<(ostream &os, CONSOLE_FG color) { return os << "\x1b[" << ((int) color + 30) << "m"; }
ostream& operator<<(ostream &os, CONSOLE_STYLE style) { return os << "\x1b[" << (style == STYLE_NONE ? 0 : 1) << "m"; }
// END CUT HERE

int minTime[1001], t, d, p[1000], ans;

map<vector<int>, int> dp;

vector<int> simplify(const vector<int> &orig) {
	vector<int> ret;
	for(int i = 0; i < len(orig); i++) {
		if(orig[i]) {
			ret.push_back(orig[i]);
		}
	}
	sort(ret.begin(), ret.end());
	return ret;
}

int brute(const vector<int> &pancakes) {
	if(pancakes.empty()) {
		return 0;
	}

	map<vector<int>, int>::iterator dpIt = dp.find(pancakes);

	if(dpIt == dp.end()) {
		// Eat 1 pancake from every
		vector<int> nextPancakesEat = pancakes;
		for(int i = 0; i < len(nextPancakesEat); i++) {
			if(nextPancakesEat[i]) {
				nextPancakesEat[i]--;
			}
		}

		int ans = 1 + brute(simplify(nextPancakesEat));

		// Split n pancakes off
		for(int splitPos = 0; splitPos < len(pancakes); splitPos++) {
			for(int n = 1; n < pancakes[splitPos]; n++) {
				vector<int> nextPancakes = pancakes;
				nextPancakes[splitPos] -= n;
				nextPancakes.push_back(n);
				ans = min(ans, 1 + brute(simplify(nextPancakes)));
			}
		}

		dpIt = dp.insert(dpIt, make_pair(pancakes, ans));
	}
	return dpIt->second;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	for(int i = 0; i <= 1000; i++) {
		// Can always eat i pancakes in i time
		minTime[i] = i;

		// Or split into 2 or more
		for(int j = 1; j < i; j++) {
			minTime[i] = min(minTime[i], 1 + max(minTime[j], minTime[i - j]));
		}

		//cout << i << ": " << minTime[i] << endl;
	}

	scanf("%d", &t);
	for(int tc = 1; tc <= t; tc++) {
		scanf("%d", &d);
		for(int i = 0; i < d; i++) {
			scanf("%d", &p[i]);
		}

		ans = 0;
		for(int i = 0; i < d; i++) {
			ans = max(ans, minTime[p[i]]);
		}

		dp.clear();
		printf("Case #%d: %d\n", tc, brute(vector<int>(p, p + d)));		
	}
	
	return 0;
}
