#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <map>
#include <set>

using namespace std;


int n, m;
int stx, sty, enx, eny;

int mp[17][17];
void Load()
{
	cin >> n >> m;
	int i, j;
	for (i = 0; i <= n + 1; i++) {
		for (j = 0; j <= m + 1; j++) {
			mp[i][j] = 1;
		}

	}
	char c;
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= m; j++) {
			c = getchar();
			while (c != '.' && c != '#' && c != 'O' && c != 'X') c = getchar();
			mp[i][j] = 0;
			if (c == '#') mp[i][j] = 1;
			if (c == 'O') {
				stx = i; sty = j;
			}
			if (c == 'X') {
				enx = i; eny = j;
			}

		}
	}
}


int dst[16][16][16][16];
int w4[16][16][4];

const int dx[4] = {0, 0, 1, -1};
const int dy[4] = {1, -1, 0, 0};


int q[2][111000][4];

void Solve()
{
	int i, j, k;
	int l;
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= m; j++) {
			for (k = 0; k < 4; k++) {
				for (l = 1; mp[i + dx[k] * l][j + dy[k] * l] == 0; l++);
				w4[i][j][k] = l - 1;	
			}
		}
	}		
	memset(dst, -1, sizeof(dst));
	int hd[2], tl[2];
	hd[0] = tl[0] = 0;
	hd[1] = 0;
	tl[1] = -1;
	q[0][hd[0]][0] = stx;
	q[0][hd[0]][1] = sty;
	q[0][hd[0]][2] = 0;
	q[0][hd[0]][3] = 0;
	dst[stx][sty][0][0] = 0;
	int pi, pj;
	int CR = 0;
	while (0==0) {
		for (hd[CR] = 0; hd[CR] <= tl[CR]; hd[CR]++) {
			i = q[CR][hd[CR]][0];
			j = q[CR][hd[CR]][1];
			pi = q[CR][hd[CR]][2];
			pj = q[CR][hd[CR]][3];
//			cerr << i << " " << j << " " << pi << " " << pj << " dst = " << dst[i][j][pi][pj] << "\n";
			for (k = 0; k < 4; k++) {
		 		if (mp[i + dx[k]][j + dy[k]] == 0 && dst[i + dx[k]][j + dy[k]][pi][pj] == -1) {
					dst[i + dx[k]][j + dy[k]][pi][pj] = dst[i][j][pi][pj] + 1;
					tl[1 - CR]++;
					q[1 - CR][tl[1 - CR]][0] = i + dx[k];
					q[1 - CR][tl[1 - CR]][1] = j + dy[k];
					q[1 - CR][tl[1 - CR]][2] = pi;
					q[1 - CR][tl[1 - CR]][3] = pj;
			 	}
			 	l = w4[i][j][k];
				if (dst[i][j][i + dx[k] * l][j + dy[k] * l] == -1) {
					dst[i][j][i + dx[k] * l][j + dy[k] * l] = dst[i][j][pi][pj];
					tl[CR]++;
					q[CR][tl[CR]][0] = i;
					q[CR][tl[CR]][1] = j;
					q[CR][tl[CR]][2] = i + dx[k] * l;
					q[CR][tl[CR]][3] = j + dy[k] * l;
				}
				if (l == 0) {
					if (pi  + pj == 0) continue;
			 		if (dst[pi][pj][pi][pj] == -1) {
						dst[pi][pj][pi][pj] = dst[i][j][pi][pj] + 1;
						tl[1 - CR]++;
						q[1 - CR][tl[1 - CR]][0] = pi;
						q[1 - CR][tl[1 - CR]][1] = pj;
						q[1 - CR][tl[1 - CR]][2] = pi;
						q[1 - CR][tl[1 - CR]][3] = pj;
				 	}
			 		if (dst[pi][pj][i][j] == -1) {
						dst[pi][pj][i][j] = dst[i][j][pi][pj] + 1;
						tl[1 - CR]++;
						q[1 - CR][tl[1 - CR]][0] = pi;
						q[1 - CR][tl[1 - CR]][1] = pj;
						q[1 - CR][tl[1 - CR]][2] = i;
						q[1 - CR][tl[1 - CR]][3] = j;
				 	}
				}
			}
		}
		hd[CR] = 0;
		tl[CR] = -1;
		CR = 1 - CR;
		hd[CR] = 0;
		if (tl[CR] == -1) break;
	}
	int res = -1;
	for (i = 0; i <= n; i++) {
		for (j = 0; j <= m; j++) {
			if (dst[enx][eny][i][j] == -1) continue;
			if (res < 0 || res > dst[enx][eny][i][j]) res = dst[enx][eny][i][j];

		}
	}
	if (res == -1) {
		cout << "THE CAKE IS A LIE\n";
	} else cout << res << "\n";


}

void Save()
{
}

int main(){
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	int nt, tt;
	cin >> nt;
	for (tt = 1; tt <= nt; tt++) {
		cout << "Case #" << tt << ": ";
		Load();
		Solve();
		Save();
	}	
	return 0;
}