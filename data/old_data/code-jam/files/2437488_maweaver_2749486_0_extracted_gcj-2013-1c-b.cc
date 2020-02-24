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

enum Dir {
	NORTH,
	SOUTH,
	EAST,
	WEST
};

ostream &operator<<(ostream &os, const Dir &dir) {
	switch(dir) {
	case NORTH: return os << "N";
	case SOUTH: return os << "S";
	case EAST: return os << "E";
	case WEST: return os << "W";
	}
}

int maxMoves;
int nextMax;
vector<Dir> curMoves;
vector<Dir> ans;
int x, y, curX, curY;
set<pair<int, int> > seen;

void dfs() {
	long long dx = abs(x - curX);
	long long dy = abs(y - curY);

	long long d = len(curMoves) + 1;
	long long minMoves = (long long) floor((sqrt(8.0L*(long double) dx+4.0L*(long double) (d*d)-4.0L*(long double)d+1.0L)-2.0L*(long double)d-1.0L)/2.0L) +
		(long long) floor((sqrt(8.0L*(long double) dy+4.0L*(long double) (d*d)-4.0L*(long double)d+1.0L)-2.0L*(long double)d-1.0L)/2.0L);

	if(len(curMoves) + minMoves > maxMoves) {
		nextMax = min(nextMax, len(curMoves) + (int) minMoves);
		return;
	}

	if(!ans.empty()) {
		return;
	}

	if(seen.count(make_pair(curX, curY))) {
		return;
	}

	if(x == curX && y == curY) {
		ans = curMoves;
		return;
	}

	curMoves.push_back(NORTH);
	curY += d;
	dfs();
	curY -= d;
	curMoves.pop_back();

	curMoves.push_back(SOUTH);
	curY -= d;
	dfs();
	curY += d;
	curMoves.pop_back();

	curMoves.push_back(EAST);
	curX += d;
	dfs();
	curX -= d;
	curMoves.pop_back();

	curMoves.push_back(WEST);
	curX -= d;
	dfs();
	curX += d;
	curMoves.pop_back();
}

bool check() {
	int curX = 0, curY = 0;
	int d = 1;
	foreach(m, ans) {
		switch(*m) {
		case NORTH: curY += d; break;
		case SOUTH: curY -= d; break;
		case EAST: curX += d; break;
		case WEST: curX -= d; break;
		}
		d++;
	}

	cout << "(" << curX << ", " << curY << ") vs (" << x << ", " << y << ")" << endl;
	return curX == x && curY == y;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	int numTc;
	cin >> numTc;
	for(int tcNum = 1; tcNum <= numTc; tcNum++) {
		cin >> x >> y;

		curX = 0;
		curY = 0;
		ans.clear();
		curMoves.clear();
		nextMax = inf;
		maxMoves = 0;
		while((curX != x || curY != y) && ans.empty()) {
			seen.clear();
			curX = 0;
			curY = 0;
			ans.clear();
			curMoves.clear();
			dfs();

			maxMoves = nextMax;
			nextMax = inf;
		}

		cout << "Case #" << tcNum << ": ";
		foreach(m, ans) {
			cout << *m;
		}
		cout << endl;
		//assert(check());
	}

	return 0;
}
