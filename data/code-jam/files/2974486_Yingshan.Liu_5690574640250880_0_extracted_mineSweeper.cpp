#include <iostream>
#include <vector>

using namespace std;

int map[50][50];
bool v[50][50];
int r, c;
int dx[8]={-1, -1, -1, 0, 0, 1, 1, 1};
int dy[8]={-1, 0, 1, -1, 1, -1, 0, 1};

void flood(int x, int y) {
	v[x][y] = true;
	if (map[x][y] == 0) {
		for (int k=0; k<8; k++) {
			int x1 = x + dx[k];
			int y1 = y + dy[k];
			if (x1>=0 && x1<r && y1>=0 && y1<c && !v[x1][y1]) flood(x1, y1);
		}	
	}
}

void print(int x, int y) {
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			if (i==x && j==y) cout << "c";
			else if (map[i][j] == -1) cout << "*";
			else cout << ".";
		}
		cout << endl;
	}
}

bool click(int x, int y) {
	if (map[x][y] == -1) return false;
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			v[i][j] = false;
		}
	}
	flood(x, y);
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			if (map[i][j]>=0 && !v[i][j]) return false;
		}
	}
	return true;
}

bool check() {
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			if (map[i][j] != -1) {
				map[i][j] = 0;
				for (int k=0; k<8; k++) {
					int x = i + dx[k];
					int y = j + dy[k];
					if (x>=0 && x<r && y>=0 && y<c && map[x][y]==-1) map[i][j]++;
				}
			}
		} 
	} 
	
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			if (click(i, j)) {
				print(i, j);
				return true;
			}
		}
	}
	return false;
}

bool dfs(int x, int y, int last) {
	if (y == c) {
		y = 0;
		x++;
	}
	if (x == r) {
		if (last == 0 && check()) return true;
		return false;
	}
	if (last > 0) {
		map[x][y] = -1;
		if (dfs(x, y+1, last-1)) return true;
	}
	map[x][y] = 0;
	if (dfs(x, y+1, last)) return true;
	
	return false;
}

void work() {
	int m;
	cin >> r >> c >> m;
	if (!dfs(0, 0, m)) cout << "Impossible" << endl;
}

int main() {
	freopen("C-small-attempt0.in", "r", stdin);
	freopen("C-small-attempt0.out", "w", stdout);
	
	int n;
	cin >> n;
	for (int i=0; i<n; i++) {
		cout << "Case #" << i+1 << ":" << endl;
		work();
	}
}