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

int t, n;
char strs[100][101];
vector<vector<pair<char, int> > > runs;

vector<pair<char, int> > getRuns(const string &str) {
	vector<pair<char, int> > ret;

	if(str.size() == 0) {
		return ret;
	}

	int curPos = 0;
	int runLen = 0;

	while(curPos < str.size()) {
		char curChar = str[curPos];
		runLen = 0;

		while(curPos < str.size() && str[curPos] == str[curPos - runLen]) {
			curPos++;
			runLen++;
		}

		ret.push_back(make_pair(curChar, runLen));
	}	

	return ret;
}   

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	scanf("%d", &t);
	for(int tc = 1; tc <= t; tc++) {
		scanf("%d", &n);
		for(int i = 0; i < n; i++) {
			scanf("%s", strs[i]);
		}

		runs.clear();
		for(int i = 0; i < n; i++) {
			runs.push_back(getRuns(string(strs[i])));
			// cout << string(strs[i]) << ": " << runs[i] << endl;
		}

		int runNum  = 0;
		bool valid = true;
		int cost = 0;
		while(true) {
			if(runNum >= len(runs[0])) {
				break;
			}

			char ch = runs[0][runNum].first;
			int maxLen = 0;
			for(int i = 0; i < n; i++) {
				// If there is no matching run or char is different can't do it
				if(runNum >= len(runs[i]) || runs[i][runNum].first != ch) {
					valid = false;
					break;
				} else {
					maxLen = max(maxLen, runs[i][runNum].second);
				}
			}

			if(!valid) {
				break;
			}

			// Could probably do fancy dp or something to find cost by why bother, we know the length
			// will be between 1 and maxLen, just try and see
			int bestCost = inf;
			for(int l = 1; l <= maxLen; l++) {
				int curCost = 0;
				for(int i = 0; i < n; i++) {
					curCost += abs(runs[i][runNum].second - l);
				}
				bestCost = min(bestCost, curCost);
			}

			cost += bestCost;
			++runNum;
		}

		if(valid) {
			// If there are any remaining unaccounted runs then not actually valid
			for(int i = 0; i < n; i++) {
				if(len(runs[i]) > runNum) {
					valid = false;
					break;
				}
			}
		}

		printf("Case #%d: ", tc);
		if(valid) {
			printf("%d\n", cost);
		} else {
			printf("Fegla Won\n");
		}

	}
	return 0;
}
