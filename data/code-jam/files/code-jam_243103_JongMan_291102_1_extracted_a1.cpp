#include<iostream>
#include<cstring>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long ll;

int r, c;
char board[15][15];
vector<pair<int,int> > pos;
int idx[15][15];
vector<int> goal;

const int dx[4] = { -1, 0, 1, 0 };
const int dy[4] = { 0, 1, 0, -1 };

struct UnionFind
{
    int n, components;
    vector<int> parent, rank;
    UnionFind(int n) : n(n), components(n), parent(n), rank(n)
    { for(int i = 0; i < n; ++i) { parent[i] = i; rank[i] = 0; } }
    int find(int here) { return parent[here] == here ? here : (parent[here] = find(parent[here])); }
    bool join(int a, int b) 
    {
        a = find(a); b = find(b);
        if(a == b) return false;
        if(rank[a] > rank[b])
            parent[b] = a;
        else 
        {
            parent[a] = b;
            if(rank[a] == rank[b]) rank[b]++;
        }
        --components;
        return true;
    }
};

int isDangerous(const VI& a)
{
    UnionFind u(a.sz);
    REP(i,a.sz) REP(j,i)
        if(abs(pos[a[i]].first - pos[a[j]].first) + abs(pos[a[i]].second - pos[a[j]].second) == 1)
            u.join(i, j);
    return u.components > 1;
}

int getPos(int y, int x)
{
    if(y < 0 || x < 0 || y >= r || x >= c) return -1;
    if(board[y][x] == '#') return -1;
    return idx[y][x];
}

int solve(VI current)
{
    sort(all(goal));
    sort(all(current));
    if(goal == current) return 0;
    set<VI> visited;
    queue<pair<int,VI> > q;
    visited.insert(current);
    q.push(make_pair(isDangerous(current), current));
    int moves = 0;
    while(q.size() > 0)
    {
        //printf("moves %d =====\n", moves);
        int qs = q.size();
        REP(qq,qs)
        {
            int dang = q.front().first;
            VI conf = q.front().second; q.pop();
          //  REP(i,conf.sz) printf("(%d,%d) ", pos[conf[i]].first, pos[conf[i]].second);
            //printf("\n");
            REP(i,conf.sz)
            {
                int block = conf[i];
                REP(k,4)
                {
                    int pushFrom = getPos(pos[block].first - dy[k], pos[block].second - dx[k]);
                    int pushTo = getPos(pos[block].first + dy[k], pos[block].second + dx[k]);
                    if(pushFrom == -1 || pushTo == -1) continue;
                    bool ok = true; 
                    REP(j,conf.sz) if(conf[j] == pushFrom || conf[j] == pushTo) { ok = false; break; }
                    if(!ok) continue;
                    VI next = conf;
                    next[i] = pushTo;
                    sort(all(next));
                    int ndang = isDangerous(next);
                    if(dang && ndang) continue;
                    if(visited.count(next)) continue;
                    if(next == goal) return moves+1;
                    visited.insert(next);
                    q.push(make_pair(ndang, next));
                }
            }
        }
        ++moves;
    }
    
    return -1;
}

int main(int argc, char* argv[])
{
    int lo = atoi(argv[1]);
    int hi = atoi(argv[2]);
    int cases;
    cin >> cases;
    for(int cc = 0; cc < cases; ++cc)
    {
        cin >> r >> c;
        REP(i,r) cin >> board[i];
        CLEAR(idx,-1);
        goal.clear();
        pos.clear();
        vector<int> current;
        REP(i,r) REP(j,c)
        {
            if(board[i][j] != '#')
            {
                idx[i][j] = pos.size();
                pos.push_back(make_pair(i, j));
            }
            if(board[i][j] == 'w' || board[i][j] == 'x')
                goal.push_back(idx[i][j]);
            if(board[i][j] == 'o' || board[i][j] == 'w')
                current.push_back(idx[i][j]);
        }
        if(lo <= cc+1 && cc+1 <= hi) 
        {
            fprintf(stderr, "%d positions\n", pos.sz);
            printf("Case #%d: %d\n", cc+1, solve(current));
        }
    }
}

