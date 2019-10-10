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

#define x second
#define y first

int N;
pair<int, int> p[1000];
vector<int> ne[10000];
int clr[1000];

bool travel(int v, int c) {
    if (clr[v] != -1) {
        if (clr[v] != c) return false;
        return true;
    }
    clr[v] = c;
    for (int i = 0; i < ne[v].size(); ++i)
        if (!travel(ne[v][i], c ^ 1))
            return false;
    return true;
}

int solve() {
    cin >> N;
    for (int i = 0; i < N; ++i)
        cin >> p[i].x >> p[i].y;
    sort(p, p + N);
    for (int i = 0; i < N; ++i) ne[i].clear();
    for (int y0 = 0; y0 <= 30; ++y0) {
        pair<int, int>* p0 = lower_bound(p, p + N, make_pair(y0 - 1, 0));
        pair<int, int>* p1 = lower_bound(p, p + N, make_pair(y0, 0));
        pair<int, int>* p2 = lower_bound(p, p + N, make_pair(y0 + 1, 0));
        pair<int, int> *i, *j;
        for (i = p1; i + 1 < p2; ++i)
            ne[i + 1 - p].push_back(i - p);
        for (i = p0, j = p1; i < p1; ++i) {
            for (; j < p2 && j->x <= i->x; ++j);
            if (j < p2) ne[j - p].push_back(i - p);
        }
        for (i = p0, j = p1; j < p2; ++j) {
            for (; i < p1 && i->x <= j->x; ++i);
            if (i < p1) ne[j - p].push_back(i - p);
        }
    }
    bool edged = false;
    for (int i = 0; i < N; ++i)
        if (!ne[i].empty())
            edged = true;
    if (!edged) return 1;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < ne[i].size(); ++j)
            if (ne[i][j] < i)
                ne[ne[i][j]].push_back(i);
    fill(clr, clr + N, -1);
    for (int i = 0; i < N; ++i)
        if (clr[i] == -1)
            if (!travel(i, 0))
                return 3;
    return 2;
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
