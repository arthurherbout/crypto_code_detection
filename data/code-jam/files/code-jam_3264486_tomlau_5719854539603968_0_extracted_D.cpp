#include <bits/stdc++.h>
using namespace std;

const int MAX = 128;

int T, N, M, r, c;
char ib[MAX][MAX], fb[MAX][MAX];

int main()
{
	cin >> T;
	for (int t = 1; t <= T; t++) { 
		cin >> N >> M;

		for (r = 1; r <= N; r++)
			for (c = 1; c <= N; c++)
				fb[r][c] = ib[r][c] = '.';

		int npr1 = -1;

		for (int i = 0; i < M; i++) {
			char ch;
			cin >> ch >> r >> c;
			fb[r][c] = ib[r][c] = ch;
			if (r == 1 && ch != '+')
				npr1 = c;
		}

		if (npr1 == -1) npr1 = N;
		for (c=1; c<=N; c++) fb[1][c] = (c != npr1) ? '+' : 'o';
		c = npr1;
		for (r=2; r<=N; r++) {
			c--;
			if (c == 0) c = N;
			fb[r][c] = 'x';
		}
		for (c=2; c<=N-1; c++)
			if (fb[N][c] != 'x') fb[N][c] = '+';
		
		if (npr1 < N-1) fb[N][npr1+1] = 'o';
		
		int points = 0, placed = 0;
		for (r = 1; r <= N; r++)
			for (c = 1; c <= N; c++) {
				if (fb[r][c] == 'o') points += 2;
				else if (fb[r][c] != '.') points += 1;
				if (fb[r][c] != ib[r][c]) placed++;
			}

		if (N > 1 && points != (N - 1) * 2 + N) {
			cout << "Error!!!  N=" << N << "  t=" << t << endl;
			for (r = 1; r <= N; r++) {
				for (c = 1; c <= N; c++)
					cout << fb[r][c];
				cout << endl;
			}
		}

		cout << "Case #" << t << ": " << points << " " << placed << endl;
		for (r = 1; r <= N; r++)
			for (c = 1; c <= N; c++)
				if (fb[r][c] != ib[r][c]) {
					cout << fb[r][c] << " " << r << " " << c << endl;
				}
		cout << endl;
	}
}