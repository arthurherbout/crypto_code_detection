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

int t, l, x;
char str[100001], res[256][256];
bool neg[256][256];

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	res['1']['1'] = '1'; neg['1']['1'] = false;
	res['1']['i'] = 'i'; neg['1']['i'] = false;
	res['1']['j'] = 'j'; neg['1']['j'] = false;
	res['1']['k'] = 'k'; neg['1']['k'] = false;

	res['i']['1'] = 'i'; neg['i']['1'] = false;
	res['i']['i'] = '1'; neg['i']['i'] = true;
	res['i']['j'] = 'k'; neg['i']['j'] = false;
	res['i']['k'] = 'j'; neg['i']['k'] = true;

	res['j']['1'] = 'j'; neg['j']['1'] = false;
	res['j']['i'] = 'k'; neg['j']['i'] = true;
	res['j']['j'] = '1'; neg['j']['j'] = true;
	res['j']['k'] = 'i'; neg['j']['k'] = false;
	
	res['k']['1'] = 'k'; neg['k']['1'] = false;
	res['k']['i'] = 'j'; neg['k']['i'] = false;
	res['k']['j'] = 'i'; neg['k']['j'] = true;
	res['k']['k'] = '1'; neg['k']['k'] = true;
	
	scanf("%d", &t);

	for(int tc = 1; tc <= t; tc++) {
		scanf("%d %d %s", &l, &x, &str);

		for(int i = 1; i < x; i++) {
			memcpy(str + i * l, str, l);
		}
		
		char sec1Value = str[0];
		bool sec1Neg = false;

		bool possible = false;
		for(int i = 1; i < l * x - 1; i++) {
#ifdef DEBUG
			for(int m = 0; m < i; m++) {
				cerr << str[m];
			}
			cerr << " = " << (sec1Neg ? "-" : "") << sec1Value << endl;
#endif
			
			if(sec1Value == 'i' && !sec1Neg) {
				char sec2Value = str[i];
				bool sec2Neg = false;

				for(int j = i + 1; j < l * x; j++) {
#ifdef DEBUG
					cerr << "  ";
					for(int m = i; m < j; m++) {
						cerr << str[m];
					}
					cerr << " = " << (sec2Neg ? "-" : "") << sec2Value << endl;
#endif
					
					if(sec2Value == 'j' && !sec2Neg) {
						char sec3Value = str[j];
						bool sec3Neg = false;

						for(int k = j + 1; k < l * x; k++) {
							if(neg[sec3Value][str[k]]) {
								sec3Neg = !sec3Neg;
							}
							sec3Value = res[sec3Value][str[k]];
						}

#ifdef DEBUG
						cerr << "    ";
						for(int m = j; m < l * x ; m++) {
							cerr << str[m];
						}
						cerr << " = " << (sec3Neg ? "-" : "") << sec3Value << endl;
#endif
						
						if(sec3Value == 'k' && !sec3Neg) {
							possible = true;
						}
					}

					if(possible) {
						break;
					} else {
						if(neg[sec2Value][str[j]]) {
							sec2Neg = !sec2Neg;
						}
						sec2Value = res[sec2Value][str[j]];
					}
				}
			}

			if(possible) {
				break;
			} else {
				if(neg[sec1Value][str[i]]) {
					sec1Neg = !sec1Neg;
				}
				sec1Value = res[sec1Value][str[i]];
			}
		}

		printf("Case #%d: %s\n", tc, possible ? "YES" : "NO");
	}
	
	return 0;
}
