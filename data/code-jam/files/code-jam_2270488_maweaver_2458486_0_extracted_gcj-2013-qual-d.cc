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
template<class T> ostream &operator<<(ostream &os, const multiset<T> &s) { os << "{ "; bool first = true; for(typename multiset<T>::const_iterator multisetIt = s.begin(); multisetIt != s.end(); ++multisetIt) { if(!first) { os << ", "; } first = false; os << *multisetIt; } return os << " }"; }
template<class T, class U> ostream &operator<<(ostream &os, const map<T, U> &m) { os << "{ "; bool first = true; for(typename map<T, U>::const_iterator mapIt = m.begin(); mapIt != m.end(); ++mapIt) { if(!first) { os << ", "; } first = false; os << mapIt->first << " = " << mapIt->second; } return os << " }"; }
// END CUT HERE

int n, k;
vector<int> req;
vector<vector<int> > gives;
vector<int> bestAns;

multiset<int> keys;
set<int> chosen;
vector<int> ans;

set<set<int> > seen;

void dfs() {
	if(seen.count(chosen)) {
		return;
	}
	seen.insert(chosen);
	//cout << "Chose " << chosen << "; keys = " << curKeys << endl;
	if(len(chosen) == n) {
		bestAns = ans;
		return;
	}

	for(int i = 0; i < n; i++) {
		if(!bestAns.empty()) {
			return;
		}

		if(!chosen.count(i) && keys.count(req[i]) > 0) {
			ans.push_back(i);

			keys.erase(keys.find(req[i]));
			for(int j = 0; j < len(gives[i]); j++) {
				keys.insert(gives[i][j]);
			}

			chosen.insert(i);

			dfs();

			ans.pop_back();
			keys.insert(req[i]);
			for(int j = 0; j < len(gives[i]); j++) {
				keys.erase(keys.find(gives[i][j]));
			}

			chosen.erase(i);
		}
	}
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	int numTc;
	cin >> numTc;
	for(int tcNum = 1; tcNum <= numTc; tcNum++) {
		cin >> k >> n;
		keys.clear();
		for(int i = 0; i < k; i++) {
			int key;
			cin >> key;
			keys.insert(key);
		}

		req.resize(n);
		gives.assign(n, vector<int>());

		for(int i = 0; i < n; i++) {
			cin >> req[i];
			int ki;
			cin >> ki;
			gives[i].resize(ki);
			for(int j = 0; j < ki; j++) {
				cin >> gives[i][j];
			}
		}

		ans.clear();
		bestAns.clear();
		chosen.clear();
		seen.clear();
		dfs();
		if(!bestAns.empty()) {
			cout << "Case #" << tcNum << ":";
			foreach(a, bestAns) {
				cout << " " << *a + 1;
			}
			cout << endl;
		} else {
			cout << "Case #" << tcNum << ": IMPOSSIBLE" << endl;
		}
	}

	return 0;
}
