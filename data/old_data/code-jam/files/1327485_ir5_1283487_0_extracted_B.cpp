#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cassert>
using namespace std;
typedef long long ll;

#define REP(i,n) for (int i=0; i<(int)(n); ++i)
#define FOR(i,k,n) for (int i=(k); i<(int)(n); ++i)
#define FOREQ(i,k,n) for (int i=(k); i<=(int)(n); ++i)
#define FORIT(i,c) for(__typeof((c).begin())i=(c).begin();i!=(c).end();++i)

#define SZ(v) (int)((v).size())
#define MEMSET(v,h) memset((v),(h),sizeof(v))

int H,W,M; 
int he[12][12];
int wa[12][12];

bool inrange(int y, int x) {
    return !(y<0 || y>=H || x<0 || x>=W);
}

int accwa(int y, int x) {
    if (inrange(y, x)) return wa[y][x];
    else return 0;
}

int dy[]={-1,0,1,0};
int dx[]={0,-1,0,1};

bool visited[12][12];
bool walled(int y, int x) {
    if (!inrange(y, x)) return false;
    if (visited[y][x]) return true;
    visited[y][x] = true;

    REP(r, 4) {
        int ay=y+dy[r], ax=x+dx[r];
        if (accwa(y, x) > accwa(ay, ax)) return false; 
        if (accwa(y, x) == accwa(ay, ax) && !walled(ay, ax)) return false;
    }

    return true;
}

void fill() {
    memcpy(wa, he, sizeof(he));

    for (;;) {
        bool inced=false;
        REP(y, H) REP(x, W) {
            MEMSET(visited, false);
            if (walled(y,x)) { wa[y][x]++; inced=true; }
        }
        if (!inced) break;
    }
}

void erose() {
    int temp[12][12];

    REP(y, H) REP(x, W) {
        temp[y][x] = he[y][x];

        int uu=8888;
        REP(r, 4) {
            int ay=y+dy[r], ax=x+dx[r];
            uu = min(uu, accwa(ay, ax));
        }

        temp[y][x] -= min(accwa(y, x)-uu, M);
    }

    memcpy(he, temp, sizeof(he));
}

void solve() {
    cin>>H>>W>>M;
    REP(y, H) REP(x, W) cin>>he[y][x];

    for (int turn=0; ; ++turn) {
        fill();
        erose();

        bool alive=false;
        REP(y, H) REP(x, W) if (he[y][x]) alive=true;
        if (!alive) {
            printf("%d\n", turn+1);
            break;
        }
    }
}

int main() {
    int total; cin>>total;
    for (int test=1; test<=total; test++) {
        printf("Case #%d: ", test);
        solve();
    }
}
