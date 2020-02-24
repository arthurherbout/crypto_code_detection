
#include <iostream>
#include <string>
#include <utility>
#include <set>

#define REP(i,N) for (int i=0; i<N; i++) 

using namespace std;


const int MAXN = 999;
bool A[MAXN][MAXN], B[MAXN][MAXN], Y[MAXN], X[MAXN], D1[MAXN], D2[MAXN];

// g1: left node to right node
// g2: right node to left node
// seen: right node seen in dfs
int g1[MAXN], g2[MAXN], seen[MAXN];


bool bipartite_match(int start, int N) {
	REP(i, 2*N-1) if (!seen[i]) {
		int i2 = i - N + 1;
		int y2 = start - i2;
		int x2 = start + i2;
		if (y2 % 2 != 0 || x2 % 2 != 0 || y2 < 0 || y2 >= 2*N || x2 < 0 || x2 >= 2*N) continue;
		seen[i] = true;
		if (g2[i] == -1 || bipartite_match(g2[i], N)) {
			g2[i] = start;
			g1[start] = i;
			return true;
		}
	}
	return false;
}


int main() {
	int T;
	char c;
	cin >> T;
	REP(cs, T) {
		int N, M;
		cin >> N >> M;
		REP(i, MAXN) Y[i] = X[i] = D1[i] = D2[i] = false;
		REP(y, N) REP(x, N) A[y][x] = B[y][x] = false;
		REP(i, M) {
			int y, x;
			cin >> c >> y >> x;
			y--; x--;
			if (c == 'x' || c == 'o') A[y][x] = Y[y] = X[x] = true;
			if (c == '+' || c == 'o') B[y][x] = D1[y+x] = D2[y-x+N-1] = true;
		}

		set<pair<int, int> > ans;
		REP(y, N) REP(x, N) if (!Y[y] && !X[x]) {
			A[y][x] = Y[y] = X[x] = true;
			ans.insert(make_pair(y, x));
		}

		int sz = 2*N-1;
		REP(i, sz) g1[i] = g2[i] = -1;
		REP(i, sz) if (!D1[i]) {
			REP(j, sz) seen[j] = D2[j];
			bipartite_match(i, N);
		}
		REP(i, sz) if (g1[i] != -1) {
			int y, x;
			y = (i + g1[i] - N+1) / 2;
			x = (i - g1[i] + N-1) / 2;
			B[y][x] = true;
			ans.insert(make_pair(y, x));
		}

		int score = 0;
		REP(y, N) REP(x, N) if (A[y][x]) score++;
		REP(y, N) REP(x, N) if (B[y][x]) score++;
 		cout << "Case #" << cs+1 << ": " << score << " " << ans.size() << endl;
 		for (set<pair<int, int> >::iterator it = ans.begin(); it != ans.end(); it++) {
 			int y = (*it).first;
 			int x = (*it).second;
 			if (A[y][x] && B[y][x]) {
 				cout << 'o' << " " << y+1 << " " << x+1 << endl;
 			} else if (A[y][x]) {
 				cout << 'x' << " " << y+1 << " " << x+1 << endl;
 			} else {
 				cout << '+' << " " << y+1 << " " << x+1 << endl;
 			}
 		}
	}
}
