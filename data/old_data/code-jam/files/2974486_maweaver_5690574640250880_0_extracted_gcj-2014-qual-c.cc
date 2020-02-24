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

int countBits(int num) {
	int cnt = 0;
	while(num) {
		cnt++;
		num = num - (num & -num);
	}
	return cnt;
}

int dx[] = {  0, -1, -1, -1,  0,  1,  1,  1 };
int dy[] = { -1, -1,  0,  1,  1,  1,  0, -1 };

bool exposesAll(const vector<vector<bool> > &board, int clickX, int clickY) {
	// cout << "Testing board\n" << board << "\nfrom (" << clickX << ", " << clickY << ")\n" << endl;
	int r = len(board);
	int c = len(board[0]);
	deque<pair<int, int> > queue;
	queue.push_back(make_pair(clickX, clickY));

	vector<vector<bool> > exposed(r, vector<bool>(c, false));
	while(!queue.empty()) {
		pair<int, int> cur = queue.front();
		queue.pop_front();

		exposed[cur.second][cur.first] = true;

		int adjCount = 0;
		for(int i = 0; i < 8; i++) {
			int nx = cur.first + dx[i];
			int ny = cur.second + dy[i];

			if(nx >= 0 && nx < c && ny >= 0 && ny < r && board[ny][nx]) {
				++adjCount;
			}
		}

		if(adjCount == 0) {
			for(int i = 0; i < 8; i++) {
				int nx = cur.first + dx[i];
				int ny = cur.second + dy[i];

				if(nx >= 0 && nx < c && ny >= 0 && ny < r && !exposed[ny][nx]) {
					queue.push_back(make_pair(nx, ny));
				}
			}
		}
	}

	// cout << exposed << endl << endl;

	for(int y = 0; y < r; y++) {
		for(int x = 0; x < c; x++) {
			if(!board[y][x] && !exposed[y][x]) {
				return false;
			}
		}
	}
	// cout << "True!" << endl;
	return true;
}

bool brute(int r, int c, int m) {
	bool any = false;
	for(int bm = 0; bm < (1 << (r * c)); ++bm) {
		if(countBits(bm) == m) {
			vector<vector<bool> > board(r, vector<bool>(c, false));
			for(int i = 0; i < r * c; i++) {
				if(bm & (1 << i)) {
					board[i % r][i / r] = true;
				}
			}

			for(int y = 0; y < r; y++) {
				for(int x = 0; x < c; x++) {
					int numAdj = 0;
					for(int i = 0; i < 8; i++) {
						int nx = x + dx[i];
						int ny = y + dy[i];
						if(nx >= 0 && nx < c && ny >= 0 && ny < r &&
						   board[ny][nx]) {

							++numAdj;
						}
					}
					
					if(!board[y][x]) {
						if(exposesAll(board, x, y)) {
							for(int py = 0; py < r; py++) {
								for(int px = 0; px < c; px++) {
									if(py == y && px == x) {
										printf("c");
									} else if(board[py][px]) {
										printf("*");
									} else {
										printf(".");
									}
								}
								printf("\n");
							}
							return true;
						}

						/*
						if(numAdj == 0) {
							break;
						}
						*/
					}
				}
			}
		}
	}

	return false;
}

bool fill(int curR, int curC, vector<vector<bool> > &board, int m) {
	// If we can finish a row or column, do it.. if not go for the bigger of the too
	bool fillRow = (m >= curC && m < curR) || (m < curC && m < curR && curC > curR);

	if(!fillRow) {
		for(int y = curR - 1; y >= 0; y--) {
			if(m > 0) {
				board[y][curC - 1] = true;
				--m;
			}
		}
		return false;
	} else {
		for(int x = curC - 1; x >= 0; x--) {
			if(m > 0) {
				board[curR - 1][x] = true;
				--m;
			}
		}
		return true;
	}
}

int t, r, c, m;

int main(__attribute__((unused)) int argc, __attribute__((unused)) char **argv) {
	/*
	scanf("%d", &t);
	for(int tc = 1; tc <= t; tc++) {
		scanf("%d %d %d", &r, &c, &m);
		vector<vector<bool> > board(r, vector<bool>(c, false));
		int origM = m;
		int origR = r;
		int origC = c;
		while(m) {
			if(fill(r, c, board, m)) {
				m = max(0, m - c);
				--r;
			} else {
				m = max(0, m - r);
				--c;
			}
		}

		bool valid = exposesAll(board, 0, 0);
		printf("Case #%d:\n", tc);
		if(!valid) {
			printf("Impossible\n");
		} else {
			for(int y = 0; y < origR; y++) {
				for(int x = 0; x < origC; x++) {
					printf("%c", (y == 0 && x == 0 ? 'c' : (board[y][x] ? '*' : '.')));
				}
				printf("\n");
			}
		}

		if(brute(origR, origC, origM, false) != valid) {
			for(int y = 0; y < origR; y++) {
				for(int x = 0; x < origC; x++) {
					printf("%c", (y == 0 && x == 0 ? 'c' : (board[y][x] ? '*' : '.')));
				}
				printf("\n");
			}

			cout << endl;
			cout << "Ooooops" << endl;
		}
	}
*/

	/*
	for(int r = 1; r <= 5; r++) {
		for(int c = 1; c <= 5; c++) {
			for(int mines = 0; mines < r * c; mines++) {
				cout << "====================" << endl;
				cout << mines << " mines in " << r << "x" << c << endl;
				brute(r, c, mines, false);
			}
		}
	}
	*/

	scanf("%d", &t);
	for(int tc = 1; tc <= t; tc++) {
		scanf("%d %d %d", &r, &c, &m);
		printf("Case #%d:\n", tc);
		if(!brute(r, c, m)) {
			printf("Impossible\n");
		}
	}
	return 0;
}
