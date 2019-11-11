#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <algorithm>
#include <queue>
#include <bitset>
#include <cmath>

using namespace std;

const int maxN = 510;
const double eps = 1e-9;

double dp[maxN][maxN][maxN][2];
double s[maxN];
double p[maxN];

void update(double& a, double b) {
	a = a > -0.5 ? min(a, b) : b;
}

void update(vector <pair<double, double> >& a, vector <pair<double, double> >& b) {
	double mxspd = -1;
	for (int i = 0; i < b.size(); ++i) {
		if (b[i].second > mxspd - eps) {
			a.clear();
			a.push_back(b[i]);
			mxspd = b[i].second;
		} else {
			a.push_back(b[i]);
		}
	}
}

double solve_clever(double y, vector <double>& p, vector <double>& s) {
	vector <pair<double, double> > pos;
	vector <pair<double, double> > neg;

	int n = p.size();
	for (int i = 0; i < n + 5; ++i) {
		for (int j = 0; j < n + 5; ++j) {
			for (int k = 0; k < n + 5; ++k) {
				dp[i][j][k][0] = dp[i][j][k][1] = -1.;
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		if (p[i] > 0) {
			pos.push_back(make_pair(p[i], s[i]));
		} else {
			neg.push_back(make_pair(p[i], s[i]));
		}
	}

	sort(pos.begin(), pos.end());
	sort(neg.begin(), neg.end());
	reverse(neg.begin(), neg.end());

	vector <pair<double, double> > npos;
	vector <pair<double, double> > nneg;

	update(npos, pos);
	update(nneg, neg);

	dp[0][0][0][0] = dp[0][0][0][1] = 0;

	//cerr << npos.size() << endl;
	//cerr << nneg.size() << endl;

	//cerr << "start" << endl;

	for (int i = 0; i <= npos.size(); ++i) {
		for (int j = 0; j <= nneg.size(); ++j) {
			for (int k = 0; k <= n; ++k) {
				if (dp[i][j][k][0] > -0.5) {
					//cerr << i << " " << j << " " << k << " " << dp[i][j][k][0] << endl;
					double dd = dp[i][j][k][0];
					//cerr << dd << endl;
					if (i + 1 <= npos.size() && k <= i) {
						double dst = k == 0 ? 0 : dd * npos[k - 1].second + npos[k - 1].first;
						double dst2 = dd * npos[i].second + npos[i].first;
						//cerr << dst << endl;
						//cerr << dst2 << endl;
						if (dst > dst2) {
							//dp[i + 1][j][k][0] = dd;
							update(dp[i + 1][j][k][0], dd);
						} else {
							//dp[i + 1][j][i + 1][0] = fabs(dst2 - dst) / (y - npos[i].second) + dd;
							update(dp[i + 1][j][i + 1][0], fabs(dst2 - dst) / (y - npos[i].second) + dd);
							//cerr << fabs(dst2 - dst) / (y - npos[i].second) + dd << endl;
						}
					}

					if (j + 1 <= nneg.size() && k <= i) {
						//cerr << "start 2" << endl;
						//cerr << k << endl;
						double dst = k == 0 ? 0 : dd * npos[k - 1].second + npos[k - 1].first;
						double dst2 = -dd * nneg[j].second + nneg[j].first;
						//cerr << i << " " << j << " " << k << " " << 0 << endl;
						//cerr << dst << endl;
						//cerr << dst2 << endl;
						//cerr << dd << endl;


						double tm = fabs(dst2 - dst) / (y - nneg[j].second) + dd;

						//cerr << tm << endl;

						int nindex = i;
						while (nindex + 1 <= npos.size()) {
							double nd = dd * npos[nindex].second + npos[nindex].first;
							if (nd < dst + eps) {
								++nindex;
							} else {
								break;
							}
						}
						update(dp[nindex][j + 1][j + 1][1], tm);
					}
				}

				if (dp[i][j][k][1] > -0.5) {
					//cerr << i << " " << j << " " << k << " " << dp[i][j][k][0] << endl;

					double dd = dp[i][j][k][1];
					//cerr << dd << endl;
					if (j + 1 <= nneg.size() && k <= j) {
						double dst = k == 0 ? 0 : -dd * nneg[k - 1].second + nneg[k - 1].first;
						double dst2 = -dd * nneg[j].second + nneg[j].first;
						if (dst < dst2) {
							update(dp[i][j + 1][k][1], dd);
						} else {
							update(dp[i][j + 1][j + 1][1], fabs(dst2 - dst) / (y - nneg[j].second) + dd);
						}
					}

					if (i + 1 <= npos.size() && k <= j) {
						double dst = k == 0 ? 0 : -dd * nneg[k - 1].second + nneg[k - 1].first;
						double dst2 = dd * npos[i].second + npos[i].first;

						double tm = fabs(dst2 - dst) / (y - npos[i].second) + dd;

						int nindex = j;
						while (nindex + 1 <= nneg.size()) {
							double nd = -dd * nneg[nindex].second + nneg[nindex].first;
							if (nd > dst - eps) {
								++nindex;
							} else {
								break;
							}
						}
						update(dp[i + 1][nindex][i + 1][0], tm);
					}
				}
			}

		}
	}

	//cerr << dp[0][1][1][1] << endl;
	//cerr << dp[1][1][1][0] << endl;
	//cerr << dp[]

	double res = 1e100;
	for (int i = 0; i <= n; ++i) {
		if (dp[npos.size()][nneg.size()][i][0] > -0.5) {
			res = min(res, dp[npos.size()][nneg.size()][i][0]);
		}
		if (dp[npos.size()][nneg.size()][i][1] > -0.5) {
			res = min(res, dp[npos.size()][nneg.size()][i][1]);
		}
	}
	return res;
}

void solve(int tcase) {
	printf("Case #%d: ", tcase);
	double y;
	vector <pair<double, double> > pos;
	vector <pair<double, double> > neg;

	cin >> y;
	int n;
	cin >> n;
	for (int i = 0; i < n; ++i) {
		cin >> p[i];
	}
	for (int i = 0; i < n; ++i) {
		cin >> s[i];
	}

	for (int i = 0; i < n + 5; ++i) {
		for (int j = 0; j < n + 5; ++j) {
			for (int k = 0; k < n + 5; ++k) {
				dp[i][j][k][0] = dp[i][j][k][1] = -1.;
			}
		}
	}

	for (int i = 0; i < n; ++i) {
		if (p[i] > 0) {
			pos.push_back(make_pair(p[i], s[i]));
		} else {
			neg.push_back(make_pair(p[i], s[i]));
		}
	}

	sort(pos.begin(), pos.end());
	sort(neg.begin(), neg.end());
	reverse(neg.begin(), neg.end());

	vector <pair<double, double> > npos;
	vector <pair<double, double> > nneg;

	update(npos, pos);
	update(nneg, neg);

	dp[0][0][0][0] = dp[0][0][0][1] = 0;

	//cerr << npos.size() << endl;
	//cerr << nneg.size() << endl;

	//cerr << "start" << endl;

	for (int i = 0; i <= npos.size(); ++i) {
		for (int j = 0; j <= nneg.size(); ++j) {
			for (int k = 0; k <= n; ++k) {
				if (dp[i][j][k][0] > -0.5) {
					cerr << i << " " << j << " " << k << " " << dp[i][j][k][0] << endl;
					double dd = dp[i][j][k][0];
					cerr << dd << endl;
					if (i + 1 <= npos.size() && k <= i) {
						double dst = k == 0 ? 0 : dd * npos[k - 1].second + npos[k - 1].first;
						double dst2 = dd * npos[i].second + npos[i].first;
						cerr << dst << " " << dst2 << endl;
						if (dst > dst2) {
							dp[i + 1][j][k][0] = dd;
						} else {
							dp[i + 1][j][i + 1][0] = fabs(dst2 - dst) / (y - npos[i].second) + dd;
						}
					}

					if (j + 1 <= nneg.size() && k <= i) {
						//cerr << "start 2" << endl;
						//cerr << k << endl;
						double dst = k == 0 ? 0 : dd * npos[k - 1].second + npos[k - 1].first;
						double dst2 = -dd * nneg[j].second + nneg[j].first;
						cerr << i << " " << j << " " << k << " " << 0 << endl;
						cerr << dst << endl;
						cerr << dst2 << endl;
						cerr << dd << endl;


						double tm = fabs(dst2 - dst) / (y - nneg[j].second) + dd;

						cerr << tm << endl;

						int nindex = i;
						while (nindex + 1 <= npos.size()) {
							double nd = dd * npos[nindex].second + npos[nindex].first;
							if (nd < dst + eps) {
								++nindex;
							} else {
								break;
							}
						}
						dp[nindex][j + 1][j + 1][1] = tm;
					}
				}

				if (dp[i][j][k][1] > -0.5) {
					//cerr << i << " " << j << " " << k << " " << dp[i][j][k][0] << endl;

					double dd = dp[i][j][k][1];
					//cerr << dd << endl;
					if (j + 1 <= nneg.size() && k <= j) {
						double dst = k == 0 ? 0 : -dd * nneg[k - 1].second + nneg[k - 1].first;
						double dst2 = -dd * nneg[j].second + nneg[j].first;
						if (dst < dst2) {
							dp[i][j + 1][k][1] = dd;
						} else {
							dp[i][j + 1][j + 1][1] = fabs(dst2 - dst) / (y - nneg[j].second) + dd;
						}
					}

					if (i + 1 <= npos.size() && k <= j) {
						double dst = k == 0 ? 0 : -dd * nneg[k - 1].second + nneg[k - 1].first;
						double dst2 = dd * npos[i].second + npos[i].first;

						double tm = fabs(dst2 - dst) / (y - npos[i].second) + dd;

						int nindex = j;
						while (nindex + 1 <= nneg.size()) {
							double nd = -dd * nneg[nindex].second + nneg[nindex].first;
							if (nd > dst - eps) {
								++nindex;
							} else {
								break;
							}
						}
						dp[i + 1][nindex][i + 1][0] = tm;
					}
				}
			}

		}
	}

	double res = 1e100;
	for (int i = 0; i <= n; ++i) {
		if (dp[npos.size()][nneg.size()][i][0] > -0.5) {
			res = min(res, dp[npos.size()][nneg.size()][i][0]);
		}
		if (dp[npos.size()][nneg.size()][i][1] > -0.5) {
			res = min(res, dp[npos.size()][nneg.size()][i][1]);
		}
	}
	printf("%.10lf\n", res);
}

double solve2(double y, const vector <double>& p, const vector <double>& s) {
	int n = p.size();
	vector <int> a(n, 0);
	for (int i = 0; i < n; ++i) {
		a[i] = i;
	}

	double mtime = 1e100;

	do {
		double ctime = 0;
		double cpos = 0;
		for (int j = 0; j < a.size(); ++j) {
			int nw = a[j];
			double ps = p[nw] > 0 ? ctime * s[nw] + p[nw] : -ctime * s[nw] + p[nw];

			double dw = fabs(ps - cpos);
			double ntime = dw / (y - s[nw]);

			ctime += ntime;
			cpos = p[nw] > 0 ? ctime * s[nw] + p[nw] : -ctime * s[nw] + p[nw];
			//cerr << cpos << endl;
		}
		if (ctime < mtime) {
		//	cout << a[0] << " " << a[1] << " " << a[2] << endl;
		}
		mtime = min(mtime, ctime);
	} while (next_permutation(a.begin(), a.end()));
	return mtime;
}

int main() {
	freopen("input.txt", "r", stdin);
	freopen("GCJ2015R3.txt", "w", stdout);

	int t;
	scanf("%d", &t);

	/*for (int i = 1; i <= t; ++i) {
		//cerr << i << endl;

		int n = rand() % 8 + 2; int y = rand() % 10 + 2;
		vector <double> p, s;
		//cin >> y >> n;
		for (int i = 0; i < n; ++i) {
			int x = rand() % 11 - 5;
			while (x == 0) {
				x = rand() % 11 - 5;
			}
			p.push_back((double)x);
		}
		sort(p.begin(), p.end());
		p.resize(unique(p.begin(), p.end()) - p.begin());
		n = p.size();
		for (int i = 0; i < n; ++i) {
			int x = rand() % (y - 1) + 1;
			//cin >> x;
			s.push_back((double)x);
		}
		//solve(i);
		printf("%.10lf\n", solve2((double)y, p, s));
		printf("%.10lf\n", solve_clever((double)y, p, s));

		double val1 = solve2((double)y, p, s);
		double val2 = solve_clever((double)y, p, s);
		if (fabs(val1 - val2) > eps) {
			cout << y << " " << n << endl;
			for (int j = 0; j < p.size(); ++j)
				cout << p[j] << " ";
			cout << endl;
			for (int j = 0; j < s.size(); ++j) {
				cout << s[j] << " ";
			}
			cout << endl;
			return 0;
		}
	}*/
	for (int i = 1; i <= t; ++i) {
			cerr << i << endl;

			int n, y;
			vector <double> p, s;
			cin >> y >> n;
			for (int i = 0; i < n; ++i) {
				double x;
				cin >> x;
				p.push_back((double)x);
			}
			for (int i = 0; i < n; ++i) {
				double x;
				cin >> x;
				s.push_back((double)x);
			}
			//solve(i);
			//printf("%.10lf\n", solve2((double)y, p, s));
			printf("Case #%d: %.10lf\n", i, solve_clever((double)y, p, s));

		}

	return 0;
}

