#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <numeric>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>
using namespace std;

#define VV vector
#define PB push_back
#define ll long long
#define ld long double
#define REP(i,n) FOR(i,0,(n)-1)
#define FOR(i,a,b) for(int i = (a);i<=(b);++i)
#define FORD(i,a,b) for(int i = (a);i>=(b);--i)
#define FORE(a,b) for(VAR(a,(b).begin());a!=(b).end();++a)
#define VAR(a,b) __typeof(b) a=(b)
#define SS(a) ((int)((a).size()))
#define ALL(x) (x).begin(),(x).end()
#define CLR(x,a) memset(x,a,sizeof(x))
#define VI VV<int>
#define VS VV<string>
int COND = 1;
#define DB(x) { if (COND > 0) { COND--; REP (xxx, 1) cerr << __LINE__ << " " << #x << " " << x << endl; cerr.flush(); } }

int N, M;

char A[41][41];
#define PII pair<int, int>
#define X first
#define Y second


int DST[41][41][41][41];
PII PR[41][41][41][41];
int dx[4] = {-1,0,1,0};
int dy[4] = {0,-1,0,1};

PII Q[41 * 41] ; int SS, EE;
void BFS(int XX[41][41], PII PP[41][41], int i, int j) {
    SS = EE = 0;
    XX[i][j] = 0; Q[EE++] = PII(i, j);

    PP[i][j] = PII(-1, -1); 
    while (SS < EE) {
        PII top = Q[SS++];
        REP (d, 4) {
            int nx = top.X + dx[d];
            int ny = top.Y + dy[d];
            if (nx >= 1 && nx <= N && ny >= 1 && ny <= M && XX[nx][ny] == -1 && A[nx][ny] != '.') {
                XX[nx][ny] = XX[top.X][top.Y] + 1;
                    ; Q[EE++] = PII(nx, ny);
                PP[nx][ny] = PII(top.X, top.Y);
            }
        }
    }
}

int solve() {
    CLR(DST, -1);
    CLR(A, 0);
    cin >> N >> M;
    VV <PII> TR;
    FOR (i, 1, N) FOR (j, 1, M) cin >> A[i][j];
    FOR (i, 1, N) FOR (j, 1, M) if (A[i][j] == 'T') {
        TR.PB(PII(i,j));
        BFS(DST[i][j], PR[i][j], i, j);
    }
    int ret = 0;
    if (TR.size() == 1) {
        FOR (i, 1, N) FOR (j, 1, M) 
            if (A[i][j] != '.') 
                ret += DST[1][1][i][j];
    }
    else {
        int VIS[41][41] = {};
        PII p = TR[1];;
        while (p != PII(1, 1)) {
            VIS[p.X][p.Y] = 1;
            ret += DST[1][1][p.X][p.Y];
            p = PR[1][1][p.X][p.Y];
        }
        
        FOR (i, 1, N) FOR (j, 1, M) if (A[i][j] != '.' && !VIS[i][j]) 
            ret += min(DST[1][1][i][j], DST[TR[1].X][TR[1].Y][i][j]);
    }
    return ret;
}

int main(int argc, char ** argv) { ios::sync_with_stdio(false);
    COND = argc >= 2 && argv[1][0] == 'q' ? (int)1e9 : 0;
    int T; cin >> T;
    FOR (my, 1, T) {
        DB(my);
        printf("Case #%d: %d\n", my, solve());
    }

    return 0;
}
