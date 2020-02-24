// another fine solution by misof
// #includes {{{
#include <algorithm>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <cmath>
#include <complex>
using namespace std;
// }}}

/////////////////// PRE-WRITTEN CODE FOLLOWS, LOOK DOWN FOR THE SOLUTION ////////////////////////////////

// pre-written code {{{
#define CLEAR(t) memset((t),0,sizeof(t))
// }}}

/////////////////// CODE WRITTEN DURING THE COMPETITION FOLLOWS ////////////////////////////////

const int MAXITEM = 10;
const int MAXCOORD = 32;
const int dr[] = {-1,1,0,0};
const int dc[] = {0,0,-1,1};
const int infty = 0x3f3f3f3f;

int R, C, STEPS, N, S, T;
vector<string> plan;
vector< pair<int,int> > soldiers, turrets;

bool can_any_turret_see[MAXCOORD][MAXCOORD];
bool can_turret_see[MAXITEM][MAXCOORD][MAXCOORD];

int soldier_dist[MAXITEM][MAXCOORD][MAXCOORD];
int memo[1<<MAXITEM][1<<MAXITEM];
pair<int,int> bestsol[1<<MAXITEM][1<<MAXITEM];

void do_turret(int t) {
    int tr, tc; tie(tr,tc) = turrets[t];
    can_turret_see[t][tr][tc] = true;
    for (int d=0; d<4; ++d) {
        int cr = tr, cc = tc;
        while (true) {
            cr += dr[d];
            cc += dc[d];
            if (cr < 0 || cr >= R || cc < 0 || cc >= C) break;
            if (plan[cr][cc] == '#') break;
            can_turret_see[t][cr][cc] = true;
        }
    }
}

void do_turret2(int t) {
    int tr, tc; tie(tr,tc) = turrets[t];
    can_any_turret_see[tr][tc] = true;
    for (int d=0; d<4; ++d) {
        int cr = tr, cc = tc;
        while (true) {
            cr += dr[d];
            cc += dc[d];
            if (cr < 0 || cr >= R || cc < 0 || cc >= C) break;
            if (plan[cr][cc] == '#') break;
            can_any_turret_see[cr][cc] = true;
        }
    }
}

void do_soldier(int s) {
    int sr, sc; tie(sr,sc) = soldiers[s];
    memset( soldier_dist[s], 0x3f, sizeof(soldier_dist[s]) );
    soldier_dist[s][sr][sc] = 0;
    queue< pair<int,int> > Q;
    Q.push( {sr,sc} );
    while (!Q.empty()) {
        int cr, cc; tie(cr,cc) = Q.front(); Q.pop();
        if (can_any_turret_see[cr][cc]) continue;
        if (soldier_dist[s][cr][cc] == STEPS) continue;
        for (int d=0; d<4; ++d) {
            int nr = cr+dr[d], nc = cc+dc[d];
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
            if (plan[nr][nc] == '#') continue;
            if (soldier_dist[s][nr][nc] != infty) continue;
            soldier_dist[s][nr][nc] = soldier_dist[s][cr][cc] + 1;
            Q.push( {nr,nc} );
        }
    }
}

int solve(int active_soldiers, int active_turrets) {
    //cout << "solve " << active_soldiers << " " << active_turrets << endl;
    int &res = memo[active_soldiers][active_turrets];
    if (res != -1) return res;
    res = 0;
    
    CLEAR(can_turret_see);
    CLEAR(can_any_turret_see);
    for (int t=0; t<T; ++t) if (active_turrets & 1<<t) { do_turret(t); do_turret2(t); }
    for (int s=0; s<S; ++s) if (active_soldiers & 1<<s) do_soldier(s);
    
    for (int r=0; r<R; ++r) for (int c=0; c<C; ++c) {
        if (!can_any_turret_see[r][c]) continue;
        for (int t=0; t<T; ++t) if (active_turrets & 1<<t) if (can_turret_see[t][r][c]) {
            for (int s=0; s<S; ++s) if (active_soldiers & 1<<s) if (soldier_dist[s][r][c] < infty) {
                int curr = solve( active_soldiers ^ 1<<s, active_turrets ^ 1<<t );
                if (1+curr > res) {
                    res = 1 + curr;
                    bestsol[active_soldiers][active_turrets] = {s,t};
                }
            }
        }
    }
    return res;
}

int main() {
    int TC; cin >> TC;
    for (int tc=1; tc<=TC; ++tc) {
        cin >> C >> R >> STEPS;
        plan.clear();
        plan.resize(R);
        for (int r=0; r<R; ++r) cin >> plan[r];

        soldiers.clear();
        turrets.clear();

        for (int r=0; r<R; ++r) for (int c=0; c<C; ++c) if (plan[r][c]=='S') { plan[r][c]='.'; soldiers.push_back({r,c}); }
        for (int r=0; r<R; ++r) for (int c=0; c<C; ++c) if (plan[r][c]=='T') { plan[r][c]='.'; turrets.push_back({r,c}); }

        S = soldiers.size();
        T = turrets.size();

        memset( memo, -1, sizeof(memo) );
        int active_soldiers = (1<<S)-1;
        int active_turrets = (1<<T)-1;
        cout << "Case #" << tc << ": " << solve(active_soldiers,active_turrets) << endl;
        while (memo[active_soldiers][active_turrets] > 0) {
            int ss, tt; tie(ss,tt) = bestsol[active_soldiers][active_turrets];
            cout << (ss+1) << " " << (tt+1) << endl;
            active_soldiers ^= 1<<ss;
            active_turrets ^= 1<<tt;
        }
    }
}
// vim: fdm=marker:commentstring=\ \"\ %s:nowrap:autoread
