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
static const long long p = 1000000007;
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
// END CUT HERE

vector<pair<int, int> > curDiamonds;
map<pair<int, int>, int> cnt;
int n, numConfigs;

void simulate(int curX, int curY) {
	bool l = false;
	foreach(d2, curDiamonds) {
		if(d2->first == curX - 1 && d2->second == curY - 1) {
			l = true;
			break;
		}
	}

	bool r = false;
	foreach(d2, curDiamonds) {
		if(d2->first == curX + 1 && d2->second == curY - 1) {
			r = true;
			break;
		}
	}

	if(curY > 0 && (!l || !r)) {
		foreach(d, curDiamonds) {
			if(d->first == curX && d->second == curY - 2) {

				// Branch if possible!
#ifdef DEBUG
				cout << "Diamond " << len(curDiamonds) << " will hit top of diamond @ (" << curX << ", " << curY - 2 << ") from " << curDiamonds << "; can go l? " << l << ", r? " << r << endl;
#endif
				if(!l) {
					simulate(curX - 1, curY - 1);
				}

				if(!r) {
					simulate(curX + 1, curY - 1);
				}

				return;
			}
		}

		if(l && !r) {
#ifdef DEBUG
			cout << "Diamond " << len(curDiamonds) << " forced to slide right" << endl;
#endif
			simulate(curX + 1, curY - 1);
		} else if(r && !l) {
#ifdef DEBUG
			cout << "Diamond " << len(curDiamonds) << " forced to slide left" << endl;
#endif
			simulate(curX - 1, curY - 1);
		} else {
			// Neither, go on down
			simulate(curX, curY - 1);
		}
	} else {
#ifdef DEBUG
		cout << "Diamond " << len(curDiamonds) << " came to a rest @ (" << curX << ", " << curY << ")" << endl;
#endif
		curDiamonds.push_back(make_pair(curX, curY));

		if(len(curDiamonds) == n) {
#ifdef DEBUG
			cout << "Final configuration: " << curDiamonds << endl;
#endif
			foreach(d, curDiamonds) {
				cnt[*d]++;
			}
			numConfigs++;
		} else {
			simulate(0, 100);
		}
		curDiamonds.pop_back();
	}
}


int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	int numTc;
	cin >> numTc;
	for(int tcNum = 1; tcNum <= numTc; tcNum++) {
		int x, y;
		cin >> n >> x >> y;

		cnt.clear();
		numConfigs = 0;
		curDiamonds.clear();
		simulate(0, 100);

		cout << "Case #" << tcNum << ": " 
		     << fixed << setprecision(16) << (long double) cnt[make_pair(x, y)] / (long double) numConfigs << endl;
		
	}

	return 0;
}
