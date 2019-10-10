#include <bits/stdc++.h>

using namespace std;

#define error(x) cout << #x << " = " << x << endl
#define pb push_back
#define sz(a) int(a.size())

typedef long long int64;
typedef pair<int, int> ii;

struct pt {
	pt(int _x = 0, int _y = 0, int _z = 0) {
		x = _x; y = _y; z = _z;
	}
	int x, y, z;
};

int st[55][33][33][33];
pt last[55][33][33][33];
bool f[55][33][33][33];
string A[55];
int R, C;

bool getBit(int state, int pos) {
	return (state >> pos) & 1;
}

int getColForbid(int c) {
	int res = 0;
	for (int i = 0; i < R; i++)
		if (A[i][c] == '#') res |= (1 << i);
	return res;
}

int getColGun(int c) {
	int res = 0;
	for (int i = 0; i < R; i++)
		if (A[i][c] == '|' || A[i][c] == '-') res |= (1 << i);
	return res;
}

bool getNextState(int x, int y, int z, vector<int> &pos, int g, int c, int &_x, int &_y, int &_z) {
	int n = sz(pos);
	_x = x; _y = y; _z = 0;
	for (int i = 0; i < n; i++) {
		int p = pos[i];
		bool ngang = !getBit(g, i);
		if (ngang) {
			_x |= (1 << p);
			_z |= (1 << p);
			if (getBit(y, p)) return 0;
		}
		if (!ngang) {
			_y |= (1 << p);
			if (getBit(z, p)) _z |= (1 << p);
			else return 0;
			for (int j = p-1; j >= 0; j--) {
				if (A[j][c] == '#') break;
				if (getBit(z, j)) _z |= (1 << j);
				for (int k = 0; k < n; k++) {
					if (pos[k] == j) return 0;
				}
			}
			for (int j = p+1; j < R; j++) {
				if (A[j][c] == '#') break;
				if (getBit(z, j)) _z |= (1 << j);
				for (int k = 0; k < n; k++) {
					if (pos[k] == j) return 0;
				}
			}
		}
	}
	for (int j = 0; j < R; j++)
		if (A[j][c] == '#') {
			_x &= (~(1 << j));
			_y &= (~(1 << j));

			if (!getBit(z, j) && !getBit(x, j))
				return 0;
			_z |= (1 << j);
		}
	// if (c == 1) error(_z), error(n);
	return 1;
}

void solve() {
	memset(f, 0, sizeof f);
	int maxState = (1 << R) - 1;
	f[0][0][0][maxState] = 1;
	for (int i = 0; i < C; i++)
	for (int x = 0; x <= maxState; x++) 
	for (int y = 0; y <= maxState; y++)
	for (int z = 0; z <= maxState; z++) {
		if (!f[i][x][y][z]) continue;
		if (i == 2) {
			// error(x); error(y); error(z);
		}

		int forbid = getColForbid(i);

		// verify forbid
		bool verify = 1;
		for (int j = 0; j < R && verify; j++)
			if (getBit(forbid, j) && (!getBit(x, j) && !getBit(z, j)))
				verify = 0;
		if (!verify) continue;
		// if (i == 2) error(verify);

		// verify gun
		int gun = getColGun(i);
		verify = 1;
		for (int j = 0; j < R && verify; j++)
			if (getBit(gun, j) && getBit(x, j))
				verify = 0;
		if (!verify) continue;
		// if (i == 2) error(verify);

		vector<int> gunPos;
		for (int j = 0; j < R; j++)
			if (getBit(gun, j)) gunPos.pb(j);
		for (int g = 0; g < (1 << sz(gunPos)); g++) {
			int _x, _y, _z;
			if (getNextState(x, y, z, gunPos, g, i, _x, _y, _z)) {
				// if (i == 1) error(_z);
				f[i+1][_x][_y][_z] = 1;
				last[i+1][_x][_y][_z] = pt(x, y, z);
				st[i+1][_x][_y][_z] = g;
			}
		}
	}

	bool verify = 0;
	for (int x = 0; x <= maxState; x++)
	for (int y = 0; y <= maxState; y++) {
		if (!f[C][x][y][maxState]) continue;
		cout << "POSSIBLE\n";
		int z = maxState;
		for (int i = C; i >= 1; i--) {
			pt trc = last[i][x][y][z];
			int g = st[i][x][y][z];
			vector<int> pos;
			for (int j = 0; j < R; j++) {
				if (A[j][i-1] == '|' || A[j][i-1] == '-') pos.pb(j);
			}
			for (int j = 0; j < sz(pos); j++) {
				int p = pos[j];
				bool ngang = !getBit(g, j);
				if (ngang) A[p][i-1] = '-';
				else A[p][i-1] = '|';
			}
			x = trc.x; y = trc.y; z = trc.z;
		}
		for (int i = 0; i < R; i++) {
			for (int j = 0; j < C-1; j++) cout << A[i][j];
			cout << "\n";
		}
		return;
	}
	cout << "IMPOSSIBLE\n";
	return;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	ios::sync_with_stdio(0);
	int T; cin >> T;
	for (int te = 1; te <= T; te++) {
		cin >> R >> C;
		C++;
		for (int i = 0; i < R; i++) {
			cin >> A[i];
			A[i] += "#";
		}
		cout << "Case #" << te << ": ";
		solve();
	}

	return 0;
}