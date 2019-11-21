bool debug = false;

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <queue>
#include <stack>
using namespace std;

template<typename T> void Low(T& a, T b) { if (b < a) a = b; }
template<typename T> void Hig(T& a, T b) { if (b > a) a = b; }

const int inf = 0x3fffffff;
const int MaxN = 500 + 10;

int N, pL[MaxN], pR[MaxN];

int M, gL[MaxN], gR[MaxN], addA[MaxN], addB[MaxN], fa[MaxN], best[MaxN][MaxN + 1];
vector<int> gA[MaxN], gB[MaxN], sons[MaxN];
bool failColoring;
int color[MaxN];

int bb[MaxN + 1];

void Update(int t, int s) {
	fill(bb, bb + N + 1, inf);
	if (s != -1) {
		for (int i = 0; i <= N; ++i) {
			for (int k = 0; k < 2; ++k) {
				int a = addA[s], b = addB[s];
				if (k) swap(a, b);
				a += i;
				b += best[s][i];
				if (b >= inf) continue;
				if (a <= N) Low(bb[a], b);
				if (b <= N) Low(bb[b], a);
			}
			if (gA[t].size() == 0 || gB[t].size() == 0) {
				int ss = gA[t].size() + gB[t].size();
				for (int i = 0; i <= ss; ++i) {
					for (int j = 0; j <= N; ++j) {
						int a = j + i;
						int b = ss - i + best[s][j];
						if (b >= inf) continue;
						if (a <= N) Low(bb[a], b);
						if (b <= N) Low(bb[b], a);
					}
				}
			}
		}
	} else {
		int a = gA[t].size(), b = gB[t].size();
		bb[a] = b;
		bb[b] = a;
		if (gA[t].size() == 0 || gB[t].size() == 0) {
			int s = gA[t].size() + gB[t].size();
			for (int i = 0; i <= s; ++i)
				Low(bb[i], s - i);
		}
	}
	for (int i = 0; i < N; ++i)
		if (bb[i] < bb[i + 1])
			bb[i + 1] = bb[i];
	for (int i = 0; i <= N; ++i)
		Hig(best[t][i], bb[i]);
}

void Calc(int v0) {
	fill(best[v0], best[v0] + N + 1, 0);
	for (vector<int>::iterator it = sons[v0].begin(); it != sons[v0].end(); ++it) {
		int &a = addA[*it], &b = addB[*it];
		a = b = 0;
		for (vector<int>::iterator itA = gA[v0].begin(); itA != gA[v0].end(); ++itA)
			if (pL[*itA] < gL[*it] && gR[*it] < pR[*itA])
				++a;
		for (vector<int>::iterator itB = gB[v0].begin(); itB != gB[v0].end(); ++itB)
			if (pL[*itB] < gL[*it] && gR[*it] < pR[*itB])
				++b;
	}
	Update(v0, -1);
	for (vector<int>::iterator it = sons[v0].begin(); it != sons[v0].end(); ++it) {
		Calc(*it);
		Update(v0, *it);
	}
}

int p[MaxN * 2];
bool mat[MaxN][MaxN];

void ColorIt(int v, int c) {
	if (color[v] != -1) {
		if (color[v] != c) failColoring = true;
		return;
	}
	color[v] = c;
	(c % 2 ? gA : gB)[c / 2].push_back(v);
	for (int i = 0; i < N; ++i)
		if (mat[v][i])
			ColorIt(i, c ^ 1);
}

int solve() {
	cin >> N;
	map<string, int> s2i;
	memset(pL, -1, sizeof(pL));
	s2i[""] = 0;
	for (int i = 1; i <= N * 2; ++i) {
		string c;
		cin >> c;
		if (s2i.count(c) == 0) {
			int v = s2i.size();
			s2i[c] = v;
		}
		p[i] = s2i[c];
		if (pL[p[i]] == -1)
			pL[p[i]] = i;
		else
			pR[p[i]] = i;
	}
	p[0] = p[N * 2 + 1] = 0;
	pL[0] = 0;
	pR[0] = N * 2 + 1;
	++N;
	memset(mat, 0, sizeof(mat));
	for (int i = 0; i < N; ++i)
		for (int j = 0; j < N; ++j)
			if (pL[i] < pL[j] && pL[j] < pR[i] && pR[i] < pR[j])
				mat[i][j] = mat[j][i] = true;
	for (int i = 0; i < N; ++i)
		gA[i].clear(), gB[i].clear();
	M = 0;
	failColoring = false;
	fill(color, color + N, -1);
	for (int i = 0; i < N; ++i)
		if (color[i] == -1) {
			ColorIt(i, M * 2 + 0);
			++M;
		}
	if (failColoring) return -1;
	for (int i = 0; i < M; ++i) {
		int &l = gL[i], &r = gR[i];
		l = N * 2 + 1;
		r = -1;
		for (vector<int>::iterator it = gA[i].begin(); it != gA[i].end(); ++it) {
			if (pL[*it] < l) l = pL[*it];
			if (pR[*it] > r) r = pR[*it];
		}
		for (vector<int>::iterator it = gB[i].begin(); it != gB[i].end(); ++it) {
			if (pL[*it] < l) l = pL[*it];
			if (pR[*it] > r) r = pR[*it];
		}
	}
	fill(fa, fa + M, -1);
	for (int i = 0; i < M; ++i)
		sons[i].clear();
	for (int i = 0; i < M; ++i)
		for (int j = 0; j < M; ++j)
			if (i != j && gL[i] < gL[j] && gR[j] < gR[i]) {
				fa[j] = i;
				sons[i].push_back(j);
			}
	addA[0] = addB[0] = 0;
	Calc(0);
	int ans = inf;
	for (int i = 0; i <= N; ++i)
		if (best[0][i] != inf && i + best[0][i] < ans)
			ans = i + best[0][i];
	if (debug) {
		cerr << "seq";
		for (int i = 0; i < N * 2; ++i)
			cerr << ' ' << p[i];
		cerr << endl;
		cerr << "group " << M << endl;
		for (int i = 0; i < M; ++i) {
			for (int j = 0; j < gA[i].size(); ++j)
				cerr << ' ' << gA[i][j];
			cerr << "  / ";
			for (int j = 0; j < gB[i].size(); ++j)
				cerr << ' ' << gB[i][j];
			cerr << endl;
		}
	}
	if (ans >= inf) return -1;
	return ans - 1;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        freopen((string(argv[1]) + ".in").c_str(), "r", stdin);
        freopen((string(argv[1]) + ".out").c_str(), "w", stdout);
    }
    int cc = 0, ccc = 0;
    for (cin >> ccc; cc < ccc; ++cc)
            cout << "Case #" << cc + 1 << ": " << solve() << endl;
    return 0;
}
