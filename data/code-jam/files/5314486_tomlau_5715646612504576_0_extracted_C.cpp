#include <bits/stdc++.h>
using namespace std;

int T, R, C, r, c;

void light(vector <string> &M)
{
	for (int r = 0; r < R; r++)
		for (int c = 0; c < C; c++)
			if (M[r][c] == '-') {
				int cc = c - 1;
				while (cc >= 0 && (M[r][cc] == ' ' || M[r][cc] == '.')) M[r][cc--] = '.';
				cc = c + 1;
				while (cc < C && (M[r][cc] == ' ' || M[r][cc] == '.')) M[r][cc++] = '.';
			}
			else if (M[r][c] == '|') {
				int rr = r - 1;
				while (rr >= 0 && (M[rr][c] == ' ' || M[rr][c] == '.')) M[rr--][c] = '.';
				rr = r + 1;
				while (rr < R && (M[rr][c] == ' ' || M[rr][c] == '.')) M[rr++][c] = '.';
			}
}

int main() {
	cin >> T;
	for (int t = 1; t <= T; t++) {
		cin >> R >> C;
		vector <string> M(R);
		vector <bool> col(C, false);
		bool ans = true;

		for (r = 0; r < R; r++) {
			cin >> M[r];
			for (c = 0; c < C; c++)
				if (M[r][c] == '|' || M[r][c] == '-') {
					M[r][c] = '*';
					col[c] = true;
				} 
				else if (M[r][c] == '.') M[r][c] = ' ';
		}

		for (r = 0; r < R; r++) {
			int last_wall = -1, last_shooter = -1;
			for (c = 0; c < C; c++)
				if (M[r][c] == '#') last_wall = c;
				else if (M[r][c] == '*') {
					if (last_shooter > last_wall) {
						M[r][last_shooter] = '|';
						M[r][c] = '|';
					}
					last_shooter = c;
				}
		}

		for (c = 0; c < C && ans; c++) {
			int last_wall = -1, last_shooter = -1;
			for (r = 0; r < R && ans; r++)
				if (M[r][c] == '#') last_wall = c;
				else if (M[r][c] == '|') last_shooter = c;
				else if (M[r][c] == '*') {
					if (last_shooter > last_wall) {
						if (M[last_shooter][c] == '|')
							ans = false;
						M[last_shooter][c] = '-';
						M[r][c] = '-';
					}
					last_shooter = r;
				}
		}

		if (ans) {
			for (r = 0; r < R; r++)
				for (c = 0; c < C; c++)
					if (M[r][c] == '*') {
						if ((r == 0 || M[r - 1][c] == '#') && (r == R - 1 || M[r + 1][c] == '#'))
							M[r][c] = '-';
						if ((c == 0 || M[r][c - 1] == '#') && (c == C - 1 || M[r][c + 1] == '#'))
							M[r][c] = '|';
					}

			light(M);

			for (r = 0; r < R && ans; r++)
				for (c = 0; c < C && ans; c++) {
					bool found = false;
					if (M[r][c] == ' ') {
						int cc = c - 1;
						while (cc >= 0 && (M[r][cc] == ' ' || M[r][cc] == '.')) cc--;;
						if (cc > 0 && M[r][cc] == '*') M[r][cc] = '|', found = true;
						cc = c + 1;
						while (cc < C && (M[r][cc] == ' ' || M[r][cc] == '.')) cc++;
						if (cc < C && M[r][cc] == '*') M[r][cc] = '|', found = true;

						int rr = r - 1;
						while (rr >= 0 && (M[rr][c] == ' ' || M[rr][c] == '.')) M[rr--][c] = '.';
						if (rr > 0 && M[rr][c] == '*') M[rr][c] = '-', found = true;
						rr = r + 1;
						while (rr < R && (M[rr][c] == ' ' || M[rr][c] == '.')) M[rr++][c] = '.';
						if (rr < R && M[rr][c] == '*') M[rr][c] = '-', found = true;

						if (!found) ans = false;
						else light(M);
					}
				}
		}


		cout << "Case #" << t << ": " << (ans ? "POSSIBLE" : "IMPOSSIBLE") << endl;
		if (ans)
			for (r = 0; r < R; r++)
				cout << M[r] << endl;
	}
}
