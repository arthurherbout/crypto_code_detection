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

int t;
long long a, b, k;
long long dp[33][2][2][2][2][2][2];

long long numWays(long long bitNum, 
                  bool belowA, bool eqA, 
                  bool belowB, bool eqB,
                  bool belowK, bool eqK,
                  const string &strA, const string &strB) {
	if(bitNum < 0) {
		if(!eqA && !eqB && !eqK) {
			return 1ll;
		} else {
			return 0ll;
		}
	}

	if(dp[bitNum][belowA][eqA][belowB][eqB][belowK][eqK] == -inf) {
		long long curNumWays = 0;

		// Can always set both a and b to 0; result will be 0, if k is 1 then below now
		string nextStrA, nextStrB;

		nextStrA = strA + "0";
		nextStrB = strB + "0";
		curNumWays += numWays(bitNum - 1ll, 
		                      belowA | (a & (1ll << bitNum)),
		                      eqA && (!(a & (1ll << bitNum))),
		                      belowB | (b & (1ll << bitNum)),
		                      eqB && (!(b & (1ll << bitNum))),
		                      belowK | (k & (1ll << bitNum)),
		                      eqK && (!(k & (1ll << bitNum))),
		                      nextStrA, nextStrB);

		// If can set a to 1, then can set a to 1 and b to 0 and result is still 0 is always valid
		if(belowA || (a & (1ll << bitNum))) {
			nextStrA = strA + "1";
			nextStrB = strB + "0";
			curNumWays += numWays(bitNum - 1,
			                      belowA,
			                      eqA && (a & (1ll << bitNum)),
			                      belowB | (b & (1ll << bitNum)),
			                      eqB && (!(b & (1ll << bitNum))),
			                      belowK | (k & (1ll << bitNum)),
			                      eqK && (!(k & (1ll << bitNum))),
			                      nextStrA, nextStrB);
		}

		// Similarly for b
		if(belowB || (b & (1ll << bitNum))) {
			nextStrA = strA + "0";
			nextStrB = strB + "1";
			curNumWays += numWays(bitNum - 1ll,
			                      belowA | (a & (1ll << bitNum)),
			                      eqA && (!(a & (1ll << bitNum))),
			                      belowB,
			                      eqB && (b & (1ll << bitNum)),
			                      belowK | (k & (1ll << bitNum)),
			                      eqK && (!(k & (1ll << bitNum))),
			                      nextStrA, nextStrB);
		}

		// Finally, if we set a and b, result will be 1 so must be able to handle that
		if((belowA || (a & (1ll << bitNum))) &&
		   (belowB || (b & (1ll << bitNum))) &&
		   (belowK || (k & (1ll << bitNum)))) {

			nextStrA = strA + "1";
			nextStrB = strB + "1";
			curNumWays += numWays(bitNum - 1ll,
			                      belowA,
			                      eqA && (a & (1ll << bitNum)),
			                      belowB,
			                      eqB && (b & (1ll << bitNum)),
			                      belowK,
			                      eqK && (k & (1ll << bitNum)),
			                      nextStrA, nextStrB);
		}

		dp[bitNum][belowA][eqA][belowB][eqB][belowK][eqK] = curNumWays;
	}
	return dp[bitNum][belowA][eqA][belowB][eqB][belowK][eqK];
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	// How many pairs of numbers are there a <= A, b <= B where a & b <= K?
	// For each bit, if have not encountered a 1 in K then a & b must be 0,
	// i.e. a or b must be 0.  And of course if have not encountered a 1
	// in a or b then they must be 0.
	//

	// Is a little easier than some because only care about <=.. so

	scanf("%d", &t);
	for(int tc = 1; tc <= t; tc++) {
		scanf("%lld %lld %lld", &a, &b, &k);
		for(int i = 0; i <= 32; i++) {
			for(int j = 0; j < 2; j++) {
				for(int k = 0; k < 2; k++) {
					for(int l = 0; l < 2; l++) {
						for(int m = 0; m < 2; m++) {
							for(int n = 0; n < 2; n++) {
								for(int o = 0; o < 2; o++) {
									dp[i][j][k][l][m][n][o] = -inf;
								}
							}
						}
					}
				}
			}
		}

		printf("Case #%d: %lld\n", tc, numWays(32, false, true, false, true, false, true, "", ""));
	}

	return 0;
}
