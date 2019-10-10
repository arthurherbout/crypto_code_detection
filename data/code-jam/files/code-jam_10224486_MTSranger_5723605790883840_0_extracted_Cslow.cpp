#include <iostream>
#include <cstring>
#include <queue>

using namespace std;

struct P {
	int r;
	int c;
	int k;
	P(){}
	P(int lr, int lc, int lk) {
		r = lr; c = lc; k = lk;
	}
};

int nr, nc;
bool grid[16][16]; // false == \, true == /
bool visited[16][16][4];
int lovers[32][2];

int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

P getLoverPos(int x) {
	if (x < nc) return P(0, x, 0);
	if (x < nc+nr) return P(x-nc, nc-1, 1);
	if (x < nc+nr+nc) return P(nr-1, nc-1-(x-nr-nc), 2);
	return P(nr-1-(x-nc-nr-nc), 0, 3);
}

bool possible(bool debug) {
	for (int i = 0; i < nr+nc; i++) {
		memset(visited, 0, sizeof visited);

		P start = getLoverPos(lovers[i][0]);
		P end = getLoverPos(lovers[i][1]);

		if (debug) {
			cerr << "start: " << start.r << " " << start.c << " " << start.k << endl;
			cerr << "end  : " << end.r << " " << end.c << " " << end.k << endl;
		}

		bool found = false;
		queue<P> q;
		q.push(start);
		while (!q.empty()) {
			P curr = q.front();
			q.pop();
			if (visited[curr.r][curr.c][curr.k]) continue;
			visited[curr.r][curr.c][curr.k] = true;
			if (debug) {
				cerr << "curr : " << curr.r << " " << curr.c << " " << curr.k << endl;
			}
			if (curr.r == end.r && curr.c == end.c && curr.k == end.k) {
				found = true;
				break;
			}

			int newk = 0;
			if (grid[curr.r][curr.c]) {
				if (curr.k == 0) newk = 3;
				else if (curr.k == 1) newk = 2;
				else if (curr.k == 2) newk = 1;
				else newk = 0;
			} else {
				if (curr.k == 0) newk = 1;
				else if (curr.k == 1) newk = 0;
				else if (curr.k == 2) newk = 3;
				else newk = 2;
			}
			q.push(P(curr.r, curr.c, newk));

			int newr = curr.r+dr[curr.k];
			int newc = curr.c+dc[curr.k];
			newk = (curr.k+2)%4;
			if (newr >= 0 && newr < nr && newc >= 0 && newc < nc) {
				q.push(P(newr, newc, newk));
			}
		}
		if (!found) return false;
	}
	return true;
}

int main() {
	int ncase; cin >> ncase;
	for (int cs = 1; cs <= ncase; cs++) {
		cin >> nr >> nc;
		for (int i = 0; i < nr+nc; i++) {
			int a, b; cin >> a >> b; a--; b--;
			lovers[i][0] = a;
			lovers[i][1] = b;

			/*
			P start = getLoverPos(lovers[i][0]);
			P end = getLoverPos(lovers[i][1]);
			cerr << start.r << " " << start.c << " " << start.k << endl;
			cerr << end.r << " " << end.c << " " << end.k << endl;
			*/
		}

		cout << "Case #" << cs << ": " << endl;
		bool found = false;
		for (int x = 0; x < (1 << nr*nc); x++) {
			for (int r = 0; r < nr; r++) {
				for (int c = 0; c < nc; c++) {
					grid[r][c] = (x & (1 << (r*nc + c))) > 0;
				}
			}
			if (possible(false)) {
				//possible(true);
				found = true;
				for (int r = 0; r < nr; r++) {
					for (int c = 0; c < nc; c++) {
						cout << (grid[r][c] ? "/" : "\\");
					}
					cout << endl;
				}
				break;
			}
		}
		if (!found) cout << "IMPOSSIBLE" << endl;
	}
}
