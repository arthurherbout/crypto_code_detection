#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <utility>
#include <iomanip>
#include <map>
#include <queue>
#include <string>
#define INF 1000000000
#define HAND_TYPE 1
#define TEST 10
#define SMALL 100
#define LARGE 1000
#define INPUT_SITUATION SMALL
#define MAKE_OUTFILE
using namespace std;
typedef long long s64;
typedef unsigned long long u64;
int T,ans;
s64 R,C,N,D;
s64 grid[200][200];
struct sq_t {
	int r, c;
	u64 b;
	bool operator<(const sq_t& other) const {
		return b > other.b;
	}
};

priority_queue<sq_t> pq;
int dx[] = {-1,1,0,0}, dy[] = {0,0,1,-1};
int main() {
	if (INPUT_SITUATION == TEST) 
		freopen("test_input.txt","r",stdin);
	else if (INPUT_SITUATION == SMALL)
		freopen("D-small.in","r",stdin);
	else if (INPUT_SITUATION == LARGE)
		freopen("D-large.in","r",stdin);
	#ifdef MAKE_OUTFILE
	freopen("output.txt","w",stdout);
	#endif
	cin >> T;
	int r, c, b;
	sq_t current;
	for (int cas=0; cas<T; cas++) {
		cin >> R >> C >> N >> D;
		for (int i=0; i<R; ++i)
			for (int j=0; j<C; ++j)
				grid[i][j] = 0;
		for (int i=0; i<N; ++i) {
			cin >> r >> c >> b;
			--r; --c;
			grid[r][c] = b;
			sq_t x;
			x.r = r;
			x.c = c;
			x.b = b;
			pq.push(x);
		}
		while (!pq.empty()) {
			current = pq.top();
			pq.pop();
			for (int d=0; d<4; ++d) {
				int x = current.r+dx[d], y=current.c+dy[d];
				if (x < 0 || x >= R || y < 0 || y >= C) continue;
				if (grid[x][y] > 0) continue;
			//	cout << "placing " << current.b+D << " at position (" << x << ", " << y << ")\n";
				grid[x][y] = current.b+D;
				sq_t next;
				next.r = x;
				next.c = y;
				next.b = grid[x][y];
				pq.push(next);
			}
		}
		/*for (int i=0; i<R; ++i) {
			for (int j=0; j<C; ++j)
				cout << grid[i][j] << ' ';
			cout << endl;
		}*/
		bool ok = true;
		for (int i=0; i<R-1; ++i)
			for (int j=0; j<C; ++j)
				if (grid[i+1][j]-grid[i][j] > D || grid[i][j]-grid[i+1][j] > D) {
					ok = false;
					/*cout << i << ',' << j << endl;
					cout << grid[i+1][j]-grid[i][j] << " ; " << D << endl;*/
				}
		for (int i=0; i<R; ++i) 
			for (int j=0; j<C-1; ++j)
				if (grid[i][j+1]-grid[i][j] > D || grid[i][j]-grid[i][j+1] > D) {
					ok = false;
					//cout << i << ',' << j << endl;
				}
		cout << "Case #" << cas+1 << ": ";
		if (ok) {
			u64 total = 0;
			for (int i=0; i<R; ++i)
				for (int j=0; j<C; ++j)
					total += grid[i][j];
			total %= 1000000007;
			cout << total << '\n';
		} else {
			cout << "IMPOSSIBLE\n";
		}
	}
	return 0;
}
