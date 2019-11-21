#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>


using namespace std;


int n, m;

char mas[22][22];

const int dx[4] = {0, 1, 0, -1};
const int dy[4] = {1, 0, -1, 0};
const int MAXSTP = 110;
const int MAXNUM = 1000;

map <int, int>  dp[22][22][MAXSTP];
set <int> was[22][22];
map <int, pair <int, int> >  prv[22][22][MAXSTP];
map <int, string> rr[22][22][MAXSTP];
                     

void Load()
{
	cin >> n >> m;
	int i,j ;
	char c;
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) {
			c = getchar();
			while (c == ' ' || c == '\n') c = getchar();
			mas[i][j] = c;
		}
	}                                                  
	int k;
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= n; j++) {
			was[i][j].clear();
			for (k = 0; k < MAXSTP; k++) {
				prv[i][j][k].clear();
				rr[i][j][k].clear();
				dp[i][j][k].clear();
			}
			dp[i][j][0][0] = -1;
			was[i][j].insert(0);
		}

	}
}



void Solve()
{
	int i, j, k;
	int d1, d2;
	int ii, jj;
	int iii, jjj;
	int res;
	string s, ss;
	int dres;
	for (k = 0; k < MAXSTP; k++) {
		for (i = 1; i <= n; i++) {
			for (j = 1; j <= n; j++) {
				for (d1 = 0; d1 < 4; d1++) {
					ii = i + dx[d1]; 
					jj = j + dy[d1];
					if (ii < 1 || ii > n || jj < 1 || jj > n) continue;
					for (d2 = 0; d2 < 4; d2++) {
						iii = ii + dx[d2]; 
						jjj = jj + dy[d2];
						if (iii < 1 || iii > n || jjj < 1 || jjj > n) continue;
						ss = "";				
						ss += mas[ii][jj]; ss += mas[iii][jjj];
						dres = mas[iii][jjj] - '0';
						if (ss[0] == '-') dres = -dres;
						map <int, int>::iterator it;
						for (it = dp[iii][jjj][k].begin(); it != dp[iii][jjj][k].end(); ++it) {
							res = it->first + dres;
							if (dp[i][j][k + 1].find(res) == dp[i][j][k + 1].end() || ss < rr[i][j][k + 1][res]) {
								if (was[i][j].find(res) == was[i][j].end()) {
									dp[i][j][k + 1][res] = it->first;
//									cerr << i << " " << j << " " << k+1 << " " << res << "\n";
									rr[i][j][k + 1][res] = ss;
									prv[i][j][k + 1][res] = make_pair(iii, jjj);
									was[i][j].insert(res);
								}	

							}
						}

					}
				}                  
			}
		}
	}
	int t;
	int ti, tj;
	for (ii = 0; ii < m; ii++) {
		ss = "";
		cin >> t;
		for (i = 1; i <= n; i++) {
			for (j = 1; j <= n; j++) {
				for (k = 0; k < MAXSTP; k++) {
					if (dp[i][j][k].find(t - mas[i][j] + '0') != dp[i][j][k].end()) {

						iii = i; jjj = j;
						s = "";
						s += mas[i][j];
						dres = t - mas[i][j] +'0';
						for (jj = k; jj > 0; jj--) {
							s += rr[iii][jjj][jj][dres];
							ti = prv[iii][jjj][jj][dres].first;
							tj = prv[iii][jjj][jj][dres].second;
							dres = dp[iii][jjj][jj][dres];
							iii = ti;
							jjj = tj;
						}
//						cerr << i << " " << j << " " << k << " " << t << "=" << s << "\n";
						if (ss.size() == 0 || ss.size() > s.size() || (ss > s && ss.size() == s.size())) ss = s;
					}
				}
			}
		}                                
		cout << ss << "\n";
	}
}



int main()
{
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
	int tt, nt;
	cin >> nt;
	for (tt = 1; tt <= nt; tt++) {
		cerr << tt << "\n";
		cout << "Case #" << tt << ":\n";
		Load();
		Solve();
	}	
	return 0;
}