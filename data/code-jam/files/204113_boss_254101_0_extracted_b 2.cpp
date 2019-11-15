#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>
#include <set>
#include <cmath>


using namespace std;
const long double eps = 1e-9;
const long double Pi = 3.1415926535897932384626433832795;


int n, m, f;
int mas[52][52];


int dp[52][52][52][2];

void Load()
{      
	cin >> n >> m >> f;
	memset(dp, -1, sizeof(dp));
	dp[1][1][1][0] = 0;
	int i, j;
	char c;
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= m; j++){
			c =getchar();
			while (c != '.' && c != '#') c = getchar();
			if (c == '.') mas[i][j] = 0;
			else mas[i][j] = 1;
		}
	}
	for (j = 1; j <= m; j++) mas[n + 1][j] = 1;

}



void Go(int i, int j, int k, int t, int res)
{
	int cpos = j;
	if (t == 1) cpos = k;
	int nf = 1;
	while(mas[i + 1][cpos] == 0) {
		i++;
		j = k = cpos;
		t = 1;
		nf++;
	}
	if (nf > f) return;
	if (dp[i][j][k][t] < 0 || dp[i][j][k][t] > res) {
		dp[i][j][k][t] = res;
	}
}


void Solve()
{
	int i, j, k, t;
	int jj, kk;
	int cpos, minp, maxp;
	for (i = 1; i < n; i++) {
		for (j = 1; j <= m; j++) {
			for (k = j; k <= m; k++) {
				for (t = 0; t <= 1; t++) {
					if (dp[i][j][k][t] < 0) continue;
					if (t == 0) cpos = j;
					else cpos = k;
					minp = cpos;
					while (minp > 1 && (mas[i][minp - 1] == 0 || minp - 1 >= j) && mas[i + 1][minp - 1] == 1 ) minp--;
					maxp = cpos;
					while (maxp < m  && (mas[i][maxp + 1] == 0 || maxp + 1 <= k) && mas[i + 1][maxp + 1] == 1) maxp++;
					if (minp > 1 && (mas[i][minp - 1] == 0 || minp - 1 >= j)) {
						Go(i + 1, minp - 1, minp - 1, 1, dp[i][j][k][t]);
					}
					if (maxp < m && (mas[i][maxp + 1] == 0|| maxp + 1 <= k)) {
						Go(i + 1, maxp + 1, maxp + 1, 1, dp[i][j][k][t]);
					}
					for (jj = minp; jj < maxp; jj++) {
						for (kk = jj; kk < maxp; kk++) {
							Go(i + 1, jj, kk, 1, dp[i][j][k][t] + kk - jj + 1);
						}
					}
					for (jj = minp + 1; jj <= maxp; jj++) {
						for (kk = jj; kk <= maxp; kk++) {
							Go(i + 1, jj, kk, 0, dp[i][j][k][t] + kk - jj + 1);
						}
					}
				}
			}
		}
	}
	int mres = -1;
	for (j = 1; j <= m; j++) {
		for (k = j; k <= m; k++) {
			for (t = 0; t <= 1; t++) {
				if (dp[n][j][k][t] < 0) continue;
				if (mres < 0 || mres > dp[n][j][k][t]) 
					mres = dp[n][j][k][t];
			}	
		}
	}
	if (mres == -1) {
		cout << "NO\n";
	} else {
		cout << "YES " << mres << "\n";
	}
}



int main()
{
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);

	int t, nt;
	cin >> nt;
	for (t = 1; t <= nt; t++) {
		Load();
		cout << "Case #" << t << ": ";
		Solve();
	}
	return 0;
}