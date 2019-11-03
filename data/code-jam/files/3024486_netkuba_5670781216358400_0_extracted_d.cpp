#include<algorithm>
#include<bitset>
#include<cassert>
#include<complex>
#include<cstdio>
#include<cstring>
#include<iomanip>
#include<iostream>
#include<map>
#include<queue>
#include<set>
#include<stack>
#include<string>
#include<vector>
#define FOR(i, a, b) for(int i =(a); i <=(b); ++i)
#define FORD(i, a, b) for(int i = (a); i >= (b); --i)
#define REP(i, n) for(int i = 0;i <(n); ++i)
#define VAR(v, i) __typeof(i) v=(i)
#define FORE(i, c) for(VAR(i, (c).begin()); i != (c).end(); ++i)
#define ALL(x) (x).begin(), (x).end()
#define SZ(x) ((int)(x).size())
#define PB push_back
#define MP make_pair
#define X first
#define Y second 
#define debug(x) {cerr <<#x <<" = " <<x <<endl; }
#define debugv(x) {{cerr <<#x <<" = "; FORE(itt, (x)) cerr <<*itt <<", "; cerr <<endl; }}
using namespace std;
typedef long long LL;
typedef long double LD;
typedef pair<int, int> PII;
typedef vector<int> VI;

template<class C> void mini(C&a4, C b4){a4=min(a4, b4); }
template<class C> void maxi(C&a4, C b4){a4=max(a4, b4); }
template<class T1, class T2>
ostream& operator<< (ostream &out, pair<T1, T2> pair) { return out << "(" << pair.X << ", " << pair.Y << ")";}

const int N = 81;
const int INF = 1e9;
int n;
int c[N];
bool vis[N];
vector<PII> e[N];

int va, vb;
typedef pair<int, bool> RES;
int depth;

int aMoves(bool bMoved);
int bMoves(bool aMoved) {
    int res = INF;
    int vvb = vb;
    int cc = c[vb];
    c[vb] = 0;

    bool moved = false;
    for (PII ee: e[vvb]) {
        if (!vis[ee.Y]) {
            vis[ee.Y] = true;
            vb = ee.X;
            mini(res, aMoves(true));
            vb = vvb;
            vis[ee.Y] = false;
            moved = true;
        }
    }

    if (!moved) {
        if (aMoved) {
            mini(res, aMoves(false));
        } else {
            mini(res, 0);
        }
    }
    c[vb] = cc;

    return res - cc;
}

int aMoves(bool bMoved) { 
    int res = -INF;
    int vva = va;
    int cc = c[va];
    c[va] = 0;

    bool moved = false;
    for (PII ee: e[vva]) {
        if (!vis[ee.Y]) {
            vis[ee.Y] = true;
            va = ee.X;
            maxi(res, bMoves(true));
            va = vva;
            vis[ee.Y] = false;
            moved = true;
        }
    }

    if (!moved) {
        if (bMoved) {
            maxi(res, bMoves(false));
        } else {
            maxi(res, 0);
        }
    }

    c[va] = cc;
    
    return res + cc;
}

int bStart() {
    int res = INF;
    REP(i, n) if (va != i) {
        vb = i;
        mini(res, aMoves(true));
    }
    return res;
}

int aStart() {
    int res = -INF;
    REP(i, n) {
        va = i;
        maxi(res, bStart());
    }
    return res;
}

void solve(int tc) {
    scanf("%d", &n);
    REP(i, n) e[i].clear();
    REP(i, n) vis[i] = false;
    
    REP(i, n) {
        scanf("%d", &c[i]);
    }
    REP(i, n-1) {
        int j; scanf("%d", &j); --j;
        e[j].PB(MP(i,i));
        e[i].PB(MP(j,i));
    }

    printf("Case #%d: %d\n", tc, aStart());
}

int main(){
    ios_base::sync_with_stdio(false);
    cout << fixed << setprecision(10);

    int t; scanf("%d", &t);
    REP(i, t) solve(i+1);
    
    return 0;
}

