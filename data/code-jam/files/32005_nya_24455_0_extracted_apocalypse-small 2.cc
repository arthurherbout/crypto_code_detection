#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <map>
#include <algorithm>
#include <iterator>
#include <functional>
#include <utility>
#include <numeric>
#include <complex>
#include <cstdio>
#include <cmath>
#include <cctype>
#include <cassert>
using namespace std;

#define REP(i,n) for(int i = 0; i < (int)(n); i++)
#define FOR(i,c) for(__typeof((c).begin()) i = (c).begin(); i != (c).end(); ++i)
#define ALLOF(c) ((c).begin()), ((c).end())

const int FOREVER = 123456789;

const int ADJ[4][2] = {
    {-1, 0},
    {0, -1},
    {0, +1},
    {+1, 0},
};

int go(const vector< vector<int> >& oldfield, int si, int sj) {
    int h = oldfield.size(), w = oldfield[0].size();
    if (oldfield[si][sj] <= 0) {
        if (0) {
            REP(i, h) {
                REP(j, w)
                    cout << max(0, oldfield[i][j]) << " ";
                cout << endl;
            }
            cout << " --> " << si << "," << sj << " ==> " << "ZERO" << endl;
        }
        return -1;
    }
    {
        bool forever = true;
        REP(dir, 4) {
            int di = ADJ[dir][0], dj = ADJ[dir][1];
            if (oldfield[si+di][sj+dj] > 0)
                forever = false;
        }
        if (forever) {
            if(0) {
                REP(i, h) {
                    REP(j, w)
                        cout << max(0, oldfield[i][j]) << " ";
                    cout << endl;
                }
                cout << " --> " << si << "," << sj << " ==> " << "FOREVER" << endl;
            }
            return FOREVER;
        }
    }
    vector< vector<int> > newfield(oldfield);
    REP(i, h) REP(j, w) if (!(i == si && j == sj) && oldfield[i][j] > 0) {
        int ti = i-1, tj = j;
        REP(dir, 4) {
            int di = ADJ[dir][0], dj = ADJ[dir][1];
            if (oldfield[i+di][j+dj] > oldfield[ti][tj]) {
                ti = i+di;
                tj = j+dj;
            }
        }
        if (oldfield[ti][tj] > 0) {
            newfield[ti][tj] -= oldfield[i][j];
        }
    }
    int res = 0;
    REP(dmg, oldfield[si][sj]+1) REP(dir, 4) {
        int di = ADJ[dir][0], dj = ADJ[dir][1];
        if (oldfield[si+di][sj+dj] > 0) {
            newfield[si+di][sj+dj] -= dmg;
            res = max(res, 1+go(newfield, si, sj));
            newfield[si+di][sj+dj] += dmg;
        }
    }

    if(0){
        REP(i, h) {
            REP(j, w)
                cout << max(0, oldfield[i][j]) << " ";
            cout << endl;
        }
        //cout << " --> " << si << "," << sj << " ==> " << res << endl;
    }

    return res;
}


void solve() {

    int h, w;
    cin >> w >> h;

    int si, sj;
    cin >> sj >> si;

    vector< vector<int> > field(h+2, vector<int>(w+2, 0));
    REP(i, h) REP(j, w)
        cin >> field[i+1][j+1];

    h += 2;
    w += 2;

    int res = go(field, si, sj);
    if (res < FOREVER) {
        cout << res << " day(s)" << endl;
    }
    else {
        cout << "forever" << endl;
    }

}


int main() {

    int nCases;
    cin >> nCases >> ws;

    REP(iCase, nCases) {

        cout << "Case #" << iCase+1 << ": ";
        solve();

    }

    return 0;
}

