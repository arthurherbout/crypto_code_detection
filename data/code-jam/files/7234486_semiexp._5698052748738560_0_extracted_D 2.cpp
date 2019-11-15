#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <iomanip>

using namespace std;
#define MOD @
#define ADD(X,Y) ((X) = ((X) + (Y)%MOD) % MOD)
typedef long long i64; typedef vector<int> ivec; typedef vector<string> svec;

int n_testcases;

int R, C, D;
char in[1010][1010];

int md[1010][1010];
int sy, sx, dy, dx;

const int addy[] = {-1, 0, 1, 0}, addx[] = {0, -1, 0, 1};

int x_pair[1020], y_pair[1020];

vector<int> another;
int imos[1010][1010];

void set_imos(int ya, int xa, int yb, int xb)
{
	// [ya, yb] * [xa, xb]
	if (ya > yb) swap(ya, yb);
	if (xa > xb) swap(xa, xb);
	
	++yb; ++xb;
	imos[ya][xa]++;
	imos[ya][xb]--;
	imos[yb][xa]--;
	imos[yb][xb]++;
}

void set_imos(pair<int, int> a, pair<int, int> b)
{
	set_imos(a.first, a.second, b.first, b.second);
}

int main()
{
	scanf("%d", &n_testcases);
	
	for (int t = 0; t < n_testcases; ++t) {
		scanf("%d%d%d", &R, &C, &D);
		for (int i = 0; i < R; ++i) scanf("%s", in[i]);
		
		for (int i = 0; i < R; ++i) {
			for (int j = 0; j < C; ++j) {
				if (in[i][j] == 'S') {
					sy = i; sx = j;
					in[i][j] = '.';
				} else if (in[i][j] == 'F') {
					dy = i; dx = j;
					in[i][j] = '.';
				}
				
				md[i][j] = -1;
			}
		}
		queue<pair<int, int> > Q;
		Q.push({sy, sx});
		md[sy][sx] = 0;
		while (!Q.empty()) {
			auto p = Q.front(); Q.pop();
			int y = p.first, x = p.second;
			for (int i = 0; i < 4; ++i) {
				int y2 = addy[i] + y, x2 = addx[i] + x;
				if (y2 >= 0 && x2 >= 0 && y2 < R && x2 < C && md[y2][x2] == -1 && in[y2][x2] == '.') {
					md[y2][x2] = md[y][x] + 1;
					Q.push({y2, x2});
				}
			}
		}
		if (D > md[dy][dx] || D % 2 != md[dy][dx] % 2 || D < abs(dy - sy) + abs(dx - sx)) {
			printf("Case #%d: IMPOSSIBLE\n", t + 1);
			continue;
		}
	//	printf("%d %d\n", D, md[dy][dx]);
		vector<pair<int, int> > sp;
		pair<int, int> pos{dy, dx};
		int dis = md[dy][dx];
		while (dis > 0) {
			int y = pos.first, x = pos.second;
			sp.push_back(pos);
			for (int i = 0; i < 4; ++i) {
				int y2 = addy[i] + y, x2 = addx[i] + x;
				if (y2 >= 0 && x2 >= 0 && y2 < R && x2 < C && md[y2][x2] == dis - 1) {
					pos = make_pair(y2, x2);
					dis--;
					break;
				}
			}
		}
		sp.push_back(pos);
		reverse(sp.begin(), sp.end());
		
		vector<pair<int, int> > dirs;
		for (int i = 1; i < sp.size(); ++i) dirs.push_back({sp[i].first - sp[i - 1].first, sp[i].second - sp[i - 1].second});

		for (int i = 0; i <= R; ++i) y_pair[i] = -1;
		for (int i = 0; i <= C; ++i) x_pair[i] = -1;
		
		int py = sy, px = sx;

		another = vector<int>(dirs.size(), -1);
		
		for (int i = 0; i < dirs.size(); ++i) {
			if (dirs[i].first == 0) {
				int xd = min(0, dirs[i].second) + px;
				if (x_pair[xd] == -1) {
					x_pair[xd] = i;
				} else {
					another[x_pair[xd]] = i;
					another[i] = x_pair[xd];
					x_pair[xd] = -1;
				}
			} else {
				int yd = min(0, dirs[i].first) + py;
				if (y_pair[yd] == -1) {
					y_pair[yd] = i;
				} else {
					another[y_pair[yd]] = i;
					another[i] = y_pair[yd];
					y_pair[yd] = -1;
				}
			}
			py += dirs[i].first;
			px += dirs[i].second;
		}
	//	for (int i = 0; i < dirs.size(); ++i) {
	//		printf("%d: %d %d, %d\n", i, dirs[i].first, dirs[i].second, another[i]);
	//	}
		
		for (int i = 0; i <= R; ++i) {
			for (int j = 0; j <= C; ++j) imos[i][j] = 0;
		}
		
		int req = (md[dy][dx] - D) / 2;
		for (int i = 0; i < dirs.size() && req > 0; ++i) {
			if (another[i] != -1 && another[i] < i) {
				set_imos(sp[another[i]], sp[i + 1]);
				--req;
			}
		}
		
		for (int i = 0; i <= R; ++i) {
			for (int j = 1; j <= C; ++j) imos[i][j] += imos[i][j - 1];
		}
		for (int i = 1; i <= R; ++i) {
			for (int j = 0; j <= C; ++j) imos[i][j] += imos[i - 1][j];
		}
		for (int i = 0; i < R; ++i) {
			for (int j = 0; j < C; ++j) if (imos[i][j] > 0) in[i][j] = '.';
		}
		//	for (auto a : dirs) printf("%d %d\n", a.first, a.second);
		printf("Case #%d: POSSIBLE\n", t + 1);
		in[sy][sx] = 'S';
		in[dy][dx] = 'F';
		for (int i = 0; i < R; ++i) puts(in[i]);
	}
	
	return 0;
}
