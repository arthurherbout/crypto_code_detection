#pragma comment (linker, "/STACK:200000000")
#define _SECURE_SCL 0
#include <algorithm>
#include <bitset>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <stack>
#include <sstream>
#include <vector>

using namespace std;


typedef long long int64;
const int INF = (int) 1E9;
const int64 INF64 = (int64) 1E18;
const double EPS = 1E-9;
const double PI = acos((double)0) * 2;

#define forn(i,n)  for (int i=0; i<int(n); ++i)
#define ford(i,n)  for (int i=int(n)-1; i>=0; --i)
#define fore(i,l,n)  for (int i=int(l); i<int(n); ++i)
#define all(a)  a.begin(), a.end()
#define fs  first
#define sc  second
#define pb  push_back
#define mp  make_pair


const int MAXN = 2100;


int n, m;
vector<int> g[MAXN];
int a[MAXN], b[MAXN];


void read() {
	cin >> n >> m;
	forn(i,m)
		scanf ("%d", &a[i]);
	forn(i,m)
		scanf ("%d", &b[i]);

	forn(i,n) {
		g[i].clear();
		g[i].pb ((i+1)%n);
		g[i].pb ((i+n-1)%n);
	}
	forn(i,m) {
		--a[i], --b[i];
		g[a[i]].pb (b[i]);
		g[b[i]].pb (a[i]);
	}
}


vector<char> u[MAXN];
vector < vector<int> > gr;
int col[MAXN];

bool check (int ans) {
	static int cu=0, u[MAXN];
	forn(i,gr.size()) {
		++cu;
		
		int cols = 0;
		forn(j,gr[i].size()) {
			int c = col[gr[i][j]];
			if (c == -1) {
				cols = INF;
				break;
			}
			if (u[c] != cu) {
				u[c] = cu;
				++cols;
			}
		}

		if (cols < ans)
			return false;
	}
	return true;
}

bool rec (int v, int ans) {
	if (v == n)
		return check (ans);

	if (! check (ans))
		return false;
	
	forn(i,ans) {
		col[v] = i;
		if (rec (v+1, ans))
			return true;
	}
	col[v] = -1;
	return false;
}

void solve() {
	forn(i,n) {
		sort (all (g[i]));
		g[i].erase (unique (all (g[i])), g[i].end());
		u[i].assign (g[i].size(), 0);
	}

	gr.clear();
	forn(i,n)
		forn(j,g[i].size())
			if (!u[i][j]) {
				gr.pb (vector<int>());
				gr.back().pb (i);

				int prev = i,  v = g[i][j];
				u[i][j] = true;
				for(;;) {
					int id = int (find (all(g[v]), prev) - g[v].begin());
					id = (id + 1) % g[v].size();

					if (u[v][id])
						break;
					gr.back().pb (v);
					u[v][id] = true;
					prev = v;
					v = g[v][id];
				}
			}

	int ans = INF;
	forn(i,gr.size())
		ans = min (ans, (int)gr[i].size());

	forn(i,n)
		col[i] = -1;
	for(;;) {
		if (rec (0, ans)) {
			printf ("%d\n", ans);
			forn(i,n)
				printf (i ? " %d" : "%d", col[i]+1);
			puts("");
			return;
		}
		--ans;

		if (ans == 0)  throw;
	}
}


int main() {
	freopen ("input.txt", "rt", stdin);
	freopen ("output.txt", "wt", stdout);

	int ts;
	cin >> ts;
	forn(test,ts) {
		read();
		printf ("Case #%d: ", test+1);
		solve();
	}

}