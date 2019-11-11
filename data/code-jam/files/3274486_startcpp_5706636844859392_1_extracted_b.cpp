#include <iostream>
#include <algorithm>
#define int long long
using namespace std;

void chmin(int &a, int b) {
	a = min(a, b);
}

int INF = 1145141919;
int t;
bool isOk[2][1440];			//isOk[0][i] = キャメロン(C)が時刻[i, i+1)に赤ちゃんを世話できるか？
int dp[1441][2][1441][2];	//交換回数最小値

void init() {
	for (int i = 0; i < 2; i++) for (int j = 0; j < 1440; j++) isOk[i][j] = true;
	for (int i = 0; i < 1441; i++) for (int j = 0; j < 2; j++) for (int k = 0; k < 1441; k++) for (int l = 0; l < 2; l++) dp[i][j][k][l] = INF;
}

void input() {
	int Ac, Aj, l, r;
	int i, j;
	
	cin >> Ac >> Aj;
	
	for (i = 0; i < Ac; i++) {
		cin >> l >> r;
		for (j = l; j < r; j++) {
			isOk[0][j] = false;
		}
	}
	for (i = 0; i < Aj; i++) {
		cin >> l >> r;
		for (j = l; j < r; j++) {
			isOk[1][j] = false;
		}
	}
}

int solve() {
	if (isOk[0][0]) dp[1][0][1][0] = 0;
	if (isOk[1][0]) dp[1][1][0][1] = 0;
	
	for (int i = 1; i < 1440; i++) {			//時刻
		for (int j = 0; j < 2; j++) {			//スタートがC:0, J:1
			for (int k = 0; k <= 720; k++) {	//キャメロンタイム
				for (int l = 0; l < 2; l++) {	//直前にC:0, J:1
					if (isOk[0][i]) {
						chmin( dp[i + 1][j][k + 1][0], dp[i][j][k][l] + (l == 1) );
					}
					if (isOk[1][i]) {
						chmin( dp[i + 1][j][k][1], dp[i][j][k][l] + (l == 0) );
					
					}
				}
			}
		}
	}
	
	int ans = INF;
	for (int j = 0; j < 2; j++) {
		for (int l = 0; l < 2; l++) {
			chmin( ans, dp[1440][j][720][l] + (j != l) );
		}
	}
	return ans;
}

signed main() {
	cin >> t;
	for (int id = 1; id <= t; id++) {
		init();
		input();
		int minCnt = solve();
		cout << "Case #" << id << ": " << minCnt << endl;
	}
	return 0;
}