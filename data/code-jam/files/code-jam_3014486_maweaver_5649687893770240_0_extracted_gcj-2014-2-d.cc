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

int t, n, m, worstCase = -inf, worstWays = 0;
char s[8][11];

void countWays(int curM, const vector<vector<string> > &curAssns) {
	if(curM == m) {
		// All subsets must be non-empty
		for(int i = 0; i < n; i++) {
			if(curAssns[i].empty()) {
				return;
			}
		}

		int cnt = 0;
		for(int i = 0; i < n; i++) {
			set<string> strs;
			
			for(int j = 0; j < len(curAssns[i]); j++) {
				// cout << curAssns[i][j] << " ";
				for(int preLen = 1; preLen <= len(curAssns[i][j]); preLen++) {
					strs.insert(curAssns[i][j].substr(0, preLen));
				}
			}

			// cout << " -> " << strs << endl;

			cnt += len(strs) + 1;
		}
		// cout << "Total: " << cnt << endl;
		// cout << endl;

		if(cnt > worstCase) {
			worstCase = cnt;
			worstWays = 1;
		} else if(cnt == worstCase) {
			++worstWays;
		}

		return;
	}

	// Pick wherer to assign current
	for(int i = 0; i < n; i++) {
		vector<vector<string> > nextAssns = curAssns;
		nextAssns[i].push_back(string(s[curM]));
		countWays(curM + 1, nextAssns);
	}
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	scanf("%d", &t);
	for(int tc = 1; tc <= t; tc++) {
		scanf("%d %d", &m, &n);

		for(int i = 0; i < m; i++) {
			scanf("%s", s[i]);
		}

		worstCase = -inf;
		worstWays = 0;
		countWays(0, vector<vector<string> >(n, vector<string>()));
		printf("Case #%d: %d %d\n", tc, worstCase, worstWays);
	}

	return 0;
}
