#include <iostream>
#include <vector>

using namespace std;

const int MAXN = 200;
int n, l;
string a[MAXN], b[MAXN];
bool changed[MAXN];

void input() {
	cin >> n >> l;
	for (int i=0; i<n; i++) {
		cin >> a[i];
	}
	for (int i=0; i<n; i++) {
		cin >> b[i];
	}
}

bool check(int x, int y) {
	for (int i=0; i<l; i++) {
		if (changed[i]) {
			if (a[x][i] == b[y][i]) return false;
		} else {
			if (a[x][i] != b[y][i]) return false;
		}
	}
	return true;
}
bool isValid() {
	for (int i=0; i<n; i++) {
		bool flag = false;
		for (int j=0; j<n; j++) {
			if(check(i, j)) {
				flag = true;
				break;
			} 
		}
		if (!flag) return false;
	}
	return true;
}

void work() {
	input();
	int ans = n+1;
	for (int i=0; i<n; i++) {
		int cnt = 0; 
		for (int j=0; j<l; j++) {
			if (a[0][j] == b[i][j]) {
				changed[j] = false; 
			} else {
				changed[j] = true;
				cnt++; 
			}
		}
		if (ans > cnt && isValid()) ans = cnt;
	}
	if (ans <= n) cout << ans << endl;
	else cout << "NOT POSSIBLE" << endl; 
}

int main() {
	freopen("A-small-attempt1.in", "r", stdin);
	freopen("A-small-attempt1.out", "w", stdout);
	
	int c;
	cin >> c;
	for (int i=0; i<c; i++) {
		cout << "Case #" << i+1 << ": "; 
		work();
	}
}