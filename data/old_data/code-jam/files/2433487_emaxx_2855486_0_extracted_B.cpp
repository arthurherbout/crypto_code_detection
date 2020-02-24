#define _CRT_SECURE_NO_DEPRECATE
#define _SECURE_SCL 0
#pragma comment (linker, "/STACK:200000000")
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

typedef long long int64;
//typedef double old_double;
//#define double long double
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


const int MAXN = 10;


int n, x[MAXN], y[MAXN];


void read() {
    cin >> n;
    forn(i, n)
        cin >> x[i] >> y[i];
}


int start, type;
int cu, u[MAXN][1 << MAXN], d[MAXN][1 << MAXN];
bool can[MAXN][MAXN];
vector<int> ans_order;

inline int area(int a, int b) {
    return (x[a] - x[b]) * (y[a] + y[b]);
}

int get_d(int prev, int msk) {
    if (msk == (1 << n) - 1) {
        if (can[prev][start])
            return area(prev, start);
        return type==0 ? INF : -INF;
    }

    int & my = d[prev][msk];
    int & myu = u[prev][msk];
    if (myu == cu)
        return my;
    myu = cu;
    my = type==0 ? INF : -INF;

    forn(next, n)
        if (!(msk & (1 << next))) {
            if (!can[prev][next])
                continue;
            int nd = get_d(next, msk + (1 << next));
            if (abs(nd) == INF)
                continue;
            nd += area(prev, next);
            if (type == 0)
                my = min(my, nd);
            else
                my = max(my, nd);
        }

    return my;
}

void restore_d(int prev, int msk) {
    ans_order.push_back(prev);
    if (msk == (1 << n) - 1) {
        if ((int)ans_order.size() != n)
            throw;
        return;
    }

    int my = get_d(prev, msk);

    forn(next, n)
        if (!(msk & (1 << next))) {
            if (!can[prev][next])
                continue;
            int nd = get_d(next, msk + (1 << next));
            if (abs(nd) == INF)
                continue;
            nd += area(prev, next);
            if (nd == my)
                return restore_d(next, msk + (1 << next));
        }

    throw;
}

inline int dist(int a, int b) {
    return (x[a] - x[b]) * (x[a] - x[b]) + (y[a] - y[b]) * (y[a] - y[b]);
}

inline double dists(int a, int b) {
    return sqrt(dist(a, b) + .0);
}


void solve() {
    forn(i, n)
        forn(j, n) {
            can[i][j] = true;
            forn(k, n)
                if (k != i && k != j && abs(dists(i, j) - dists(i, k) - dists(j, k)) < 1E-9)
                    can[i][j] = false;
        }

    int ans = 0;
    ans_order.clear();
    for (start = 0; start < n; ++start) {
        for (type = 0; type < 2; ++type) {
            ++cu;
            int cans = abs(get_d(start, 1 << start));
            if (cans > ans && cans != INF) {
                ans = cans;
                ans_order.clear();
                restore_d(start, 1 << start);
            }
        }
    }

    if (ans_order.empty())
        throw;

    forn(i, n)
        printf (" %d", ans_order[i]);
    puts("");
}


int main(int argc, char* argv[]) {
#ifdef SU2_PROJ
	freopen("input.txt", "rt", stdin);
	freopen("output.txt", "wt", stdout);
#endif

    int desired_test;
    if (argc < 2) {
        desired_test = -1;
        cerr << "Desired all tests." << endl;
    }
    else {
        desired_test = atoi(argv[1]);
        cerr << "Desired only test #" << desired_test << "." << endl;
    }

	int tests;
	cin >> tests;
	for (int test = 1; test <= tests; ++test) {
		read();
		if (!cin)
            throw runtime_error("Failed to read test!");

        if (desired_test != -1 && test != desired_test)
            continue;

        cerr << "Executing test #" << test << "..." << endl;
        cout << "Case #" << test << ":";
		solve();
	}
    cerr << "Finish." << endl;
}
