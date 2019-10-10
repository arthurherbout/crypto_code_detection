#include <iostream>
#include <vector>
#include <queue>
using namespace std;

struct state {
	int x;
	int y;
	int h;
	state(int x, int y, int h) : x(x), y(y), h(h) {}

	bool operator<(const state& a) const {
		if (h > a.h) return true;
		if (h < a.h) return false;
		return false;
		if (x != a.x) return x < a.x;
		if (y != a.y) return y < a.y;
		return y < a.y;
	}
};

class solver {
	int H;
	int N;
	int M;
	vector<vector<int> > hi;
	vector<vector<int> > low;
	vector<vector<int> > time;
	priority_queue<state> Q;
public:
	solver(int H, int N, int M, vector<vector<int> > hi, vector<vector<int> > low)
		: H(H), N(N), M(M), hi(N, vector<int>(M)), low(N, vector<int>(M)), time(N, vector<int>(M, -1000000)) {
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				this->hi[i][j] = hi[i][j];
				this->low[i][j] = low[i][j];
			}
		}
	}

	void ini(int x, int y) {
		if (time[x][y] == H) { return; }
		int dx[] = {0,0,1,-1};
		int dy[] = {1,-1,0,0};

		time[x][y] = H;
		state ist(x, y, H);
		Q.push(ist);
		for (int k = 0; k < 4; k++) {
			int nx = x+dx[k];
			int ny = y+dy[k];
			if (nx < 0 || nx >= N || y < 0 || y >= M) { continue; }
			int nhi = hi[nx][ny];
			int nlow = low[nx][ny];
			int chi = hi[x][y];
			int clow = low[x][y];
			if (min(nhi, chi) >= max(H, max(nlow, clow)) + 50) {
				ini(nx, ny);
			}
		}
	}

	double solve() {
		int dx[] = {0,0,1,-1};
		int dy[] = {1,-1,0,0};

		ini(0, 0);
		while (!Q.empty()) {
			state top = Q.top();
			int x = top.x;
			int y = top.y;
			int h = top.h;
			Q.pop();
			if (x == N-1 && y == M-1) { break; }
			//cerr << x << "," << y << "," << h << endl;
			for (int k = 0; k < 4; k++) {
				int nx = x + dx[k];
				int ny = y + dy[k];
				if (nx < 0 || nx >= N || y < 0 || y >= M) { continue; }
				int nhi = hi[nx][ny];
				int nlow = low[nx][ny];
				int chi = hi[x][y];
				int clow = low[x][y];
				if (min(nhi, chi) < max(nlow, clow) + 50) { continue; }
				int ch = min(h, min(nhi, chi) - 50);
				int nh = ch - (ch - clow >= 20 ? 10 : 100);
				if (time[nx][ny] < nh) {
					time[nx][ny] = nh;
					state nst(nx, ny, nh);
					Q.push(nst);
				}
			}
		}
		return (double)(H - time[N-1][M-1]) / 10;
	}
};

int main() {
	int T; cin >> T;
	for (int No = 1; No <= T; No++) {
		int H, N, M; cin >> H >> N >> M;
		vector<vector<int> > hi(N, vector<int>(M));
		vector<vector<int> > low(N, vector<int>(M));
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> hi[i][j];
			}
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				cin >> low[i][j];
			}
		}

		if (No >= 39) {
			solver prob(H, N, M, hi, low);

			double ans = prob.solve();
			cout << "Case #" << No << ": " << ans << endl;
		}
	}
	return 0;
}
