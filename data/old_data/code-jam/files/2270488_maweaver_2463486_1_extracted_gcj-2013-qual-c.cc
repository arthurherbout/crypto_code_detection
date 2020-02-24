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

vector<int> intToDigits(unsigned long long number) {
	vector<int> digits;
	
	while(number) {
		digits.push_back(number % 10);
		number /= 10;
	}
	
	reverse(digits.begin(), digits.end());
	return digits;
}

unsigned long long digitsToInt(const vector<int> &digits) {
	unsigned long long ret = 0;
	for(int i = 0; i < (int) digits.size(); i++) {
		ret *= 10;
		ret += digits[i];
	}
	return ret;
}

vector<string> pals;
vector<unsigned long long> fairSquares;

void genPals(const string &cur, int goalLen, bool first) {
	if(2 * len(cur) >= goalLen) {
		string rev = cur;
		reverse(rev.begin(), rev.end());
		if(2 * len(cur) > goalLen) {
			rev = rev.substr(1);
		}

		pals.push_back(cur + rev);
		return;
	}

	for(int i = (first ? 1 : 0); i <= 9; i++) {
		genPals(cur + (char) ('0' + i), goalLen, false);
	}
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	for(int i = 1; i <= 10; i++) {
		pals.clear();
		genPals("", i, true);
		foreach(pal, pals) {
			istringstream palParser(*pal);
			unsigned long long s;
			palParser >> s;
			s = s * s;

			ostringstream sStream;
			sStream << s;

			string forward = sStream.str();
			string rev = forward;
			reverse(rev.begin(), rev.end());

			if(forward == rev) {
				fairSquares.push_back(s);
			}
		}
	}
	
	sort(fairSquares.begin(), fairSquares.end());

	int numTc;
	cin >> numTc;
	for(int tcNum = 1; tcNum <= numTc; tcNum++) {
		unsigned long long a, b;
		cin >> a >> b;

		vector<unsigned long long>::iterator aIt = lower_bound(fairSquares.begin(), fairSquares.end(), a);
		vector<unsigned long long>::iterator bIt = lower_bound(fairSquares.begin(), fairSquares.end(), b);

		if(*bIt > b) {
			--bIt;
		}

		cout << "Case #" << tcNum << ": " << bIt - aIt + 1 << endl;
	}

	return 0;
}
