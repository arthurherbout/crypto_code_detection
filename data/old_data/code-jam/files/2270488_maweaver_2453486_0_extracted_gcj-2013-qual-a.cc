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

bool won(const vector<string> &board, int x, int y, int dx, int dy, char ch) {
	bool any = false;
	for(int i = 0; i < 4; i++) {
		if(board[y + i * dy][x + i * dx] == ch) {
			any = true;
		} else if(board[y + i * dy][x + i * dx] != 'T') {
			return false;
		}
	}

	return any;
}

bool playerWon(const vector<string> &board, char ch) {
	return won(board, 0, 0, 1, 0, ch) ||
		won(board, 0, 1, 1, 0, ch) ||
		won(board, 0, 2, 1, 0, ch) ||
		won(board, 0, 3, 1, 0, ch) ||
		won(board, 0, 0, 0, 1, ch) ||
		won(board, 1, 0, 0, 1, ch) ||
		won(board, 2, 0, 0, 1, ch) ||
		won(board, 3, 0, 0, 1, ch) ||
		won(board, 0, 0, 1, 1, ch) ||
		won(board, 3, 0, -1, 1, ch);
}

bool over(const vector<string> &board) {
	for(int y = 0; y < 4; y++) {
		for(int x = 0; x < 4; x++) {
			if(board[y][x] == '.') {
				return false;
			}
		}
	}
	return true;
}

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {

	int numTc;
	cin >> numTc;
	for(int tcNum = 1; tcNum <= numTc; tcNum++) {
		vector<string> board(4);
		for(int i = 0; i < 4; i++) {
			cin >> board[i];
		}

		cout << "Case #" << tcNum << ": ";
		if(playerWon(board, 'X')) {
			cout << "X won" << endl;
		} else if(playerWon(board, 'O')) {
			cout << "O won" << endl;
		} else if(!over(board)) {
			cout << "Game has not completed" << endl;
		} else {
			cout << "Draw" << endl;
		}
	}
	
	return 0;
}
