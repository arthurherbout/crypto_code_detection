#pragma comment(linker, "/STACK:16000000")
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(int i=0;i<n;i++)
#define F1(i,n) for(int i=1;i<=n;i++)
const int MOD = 1000002013;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

const int N = 1001;
int i, j, k, m, n, l, good;
int S;
int x[N], y[N], z[N];
int vx[N], vy[N], vz[N], v[N];
ll A[N][N], B[N][N], C[N][N];
double t1[N][N], t2[N][N], e[N][N];
vector< pair<double, double> > u[N];
double P, Q, R;
int p2(int x) { return x * x; }

void dfs(int i, double L, double R) {
	if (!u[1].empty()) good = 1;
	if (good) return;

	F0(k, SZ(u[i])) {
		if (L >= u[i][k].first - eps && R <= u[i][k].second + eps) return;
	}

	int j = 0;
	while (j < SZ(u[i])) {
		if (u[i][j].first > R || u[i][j].second < L) {
			j++;
			continue;
		}
		L = min(L, u[i][j].first);
		R = max(R, u[i][j].second);
		u[i].erase(u[i].begin() + j);
	}
	u[i].push_back(make_pair(L, R));
	sort(u[i].begin(), u[i].end());

	F0(j, n) if (i != j) {
		double LL = max(L, t1[i][j]);
		double RR = min(R, t2[i][j]);
		if (LL < 0) LL = 0;
		if (LL <= RR) dfs(j, LL, RR + S);
	}
}

int ok() {
	F0(i, n) F0(j, n) {
		if (i < j) {
			t1[i][j] = t2[i][j] = -1.0;
			if (A[i][j] != 0) {
				double D2 = B[i][j] * B[i][j] - 4 * A[i][j] * (C[i][j] - R*R);
				if (D2 >= 0) {
					double D = sqrt(D2);
					t1[i][j] = (-B[i][j] - D) / (2 * A[i][j]);
					t2[i][j] = (-B[i][j] + D) / (2 * A[i][j]);
				}
			}
			else {
				if (C[i][j] <= R * R) {
					t1[i][j] = 0.0; t2[i][j] = 100000.0;
				}
			}
		}
		t2[i][j] = min(t2[i][j], 10000.0);
	}
	F0(i, n) F0(j, n) {
		if (i > j) {
			t1[i][j] = t1[j][i];
			t2[i][j] = t2[j][i];
		}
	}
	F0(i, n) u[i].clear();

	F0(i, n) F0(j, n) e[i][j] = -1.0;
	good = 0;
	dfs(0, 0, S);
	return good;
}

int main() {
    //freopen("x.in", "r", stdin);

	//freopen("C-small-attempt0.in", "r", stdin);
	//freopen("C-small-attempt0.out", "w", stdout);

	freopen("C-large.in", "r", stdin);
	freopen("C-large3.out", "w", stdout);

	int tt, tn; cin >> tn;

	F1(tt,tn) {
		cerr << tt << endl;
		cin >> n >> S;
		F0(i, n) {
			cin >> x[i] >> y[i] >> z[i];
			cin >> vx[i] >> vy[i] >> vz[i];
		}

		F0(i, n) F0(j, n) {
			A[i][j] = p2(vx[i] - vx[j]) + p2(vy[i] - vy[j]) + p2(vz[i] - vz[j]);
			B[i][j] = (vx[i] - vx[j]) * (x[i] - x[j]) + (vy[i] - vy[j]) * (y[i] - y[j]) + (vz[i] - vz[j]) * (z[i] - z[j]);
			B[i][j] *= 2;
			C[i][j] = p2(x[i] - x[j]) + p2(y[i] - y[j]) + p2(z[i] - z[j]);
		}

		P = 0.0; Q = 2000.0;
		F0(it, 50) {
			R = (P + Q) / 2;
			if (ok()) Q = R; else P = R;
		}

		printf("Case #%d: %.6lf\n", tt, P);
	}
	return 0;
}
