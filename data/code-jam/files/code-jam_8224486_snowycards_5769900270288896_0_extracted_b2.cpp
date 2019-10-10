#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <algorithm>
#include <vector>
#include <queue>
#include <stack>
#include <map>

using namespace std;

#define rep(i,n) for(int i = 0, _##i = n; i < _##i; ++i)
#define dwn(i,n) for(int i = n; i >= 0; --i)
#define itr(v, it) for(__typeof(v.begin()) it = v.begin(); it != v.end(); it++)
#define max(a,b) (a>b?a:b)

#define vi vector<int>

#define INF (int) 1e9
#define EPS 1e-9


void solve() {
	int grid[16][16];
	memset(grid, 1, sizeof(int) * 16 * 16);
	int r, c, n;
	cin >> r >> c >> n;
	int removes = r*c-n;

	int collisions = 4;

	while(collisions != 0) {
		if(removes == 0)
			break;
		for(int i = 0; i < r; i++) {
			bool stop = false;
			for(int j = 0; j < c; j++) {
				int localCols = 0;
				if(i > 0 && grid[i-1][j]) localCols++;
				if(j > 0 && grid[i][j-1]) localCols++;
				if(i < r-1 && grid[i+1][j]) localCols++;
				if(j < c-1 && grid[i][j+1]) localCols++;

				if(localCols == collisions) {
					removes--;
					grid[i][j] = 0;
				}
				if(removes == 0) {
					stop = true;
					break;
				}
			}
			if(stop)
				break;
		}
		collisions--;
	}

	int realCols = 0;
	for(int i = 0; i < r-1; i++) {
		for(int j = 0; j < c; j++) {
			if(grid[i][j]) {
				if(grid[i+1][j]) realCols++;
			}
		}
	}
for(int i = 0; i < r; i++) {
		for(int j = 0; j < c-1; j++) {
			if(grid[i][j]) {
				if(grid[i][j+1]) realCols++;
			}
		}
	}


	int grid2[16][16];
	memset(grid2, 0, sizeof(int) * 16 * 16);

	for(int i = 0; i < r; i++) {
		int buff = i % 2;
		for(int j = buff; j < c; j += 2) {
			grid2[i][j] = 1;
			n--;
		}
	}



	if(n > 0) {
		bool stop = false;
		for(collisions = 1; collisions < 5; collisions++) {
		for(int i = 0; i < r; i++) {
			for(int j = 0; j < c; j++) {
				if(!grid2[i][j] && n > 0) {
					int localCols = 0;
					if(i > 0 && grid2[i-1][j]) localCols++;
				if(j > 0 && grid2[i][j-1]) localCols++;
				if(i < r-1 && grid2[i+1][j]) localCols++;
				if(j < c-1 && grid2[i][j+1]) localCols++;
					if(localCols == collisions) {
						grid2[i][j] = 1;
						n--;
					}
				if(n == 0)
					stop = true;
				if(stop) break;
			}
			if(n == 0)
				stop = true;
			if(stop) break;
		}
		if(stop) break;
	}
	if(stop) break;
	}
}

	int realCols2 = 0;
	for(int i = 0; i < r-1; i++) {
		for(int j = 0; j < c; j++) {
			if(grid2[i][j]) {
				if(grid2[i+1][j]) realCols2++;
			}
		}
	}
for(int i = 0; i < r; i++) {
		for(int j = 0; j < c-1; j++) {
			if(grid2[i][j]) {
				if(grid2[i][j+1]) realCols2++;
			}
		}
	}



	realCols = realCols < realCols2 ? realCols : realCols2;

	printf("%d\n", realCols);
}

int main() {

	int cases;
	cin >> cases;

	rep(current, cases) {

		printf("Case #%d: ", current+1);
		solve();
	}

	return 0;
}