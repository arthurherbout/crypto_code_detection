#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<iostream>
#include<cstdio>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

int h, w;
string map[11];
int cache[11][1040];
int broken[11];

int go(int y, int prev_row)
{
    if(y == h) return 0;
    int& ret = cache[y][prev_row]; if(ret != -1) return ret;
    ret = 0;
    REP(this_row,(1<<w))
    {
        if(this_row & broken[y]) continue;
        bool ok = true;
        REP(i,w) if((this_row & (3<<i)) == (3<<i)) ok = false;
        if(!ok) continue;
        if(this_row & (prev_row<<1)) continue;
        if(this_row & (prev_row>>1)) continue;
        ret >?= __builtin_popcount(this_row) + go(y+1, this_row);
    }
    return ret;
}

int main()
{
    int cases;
    cin >> cases;
    REP(cc,cases)
    {
        cin >> h >> w;
        REP(i,h) cin >> map[i];
        CLEAR(broken,0);
        CLEAR(cache,-1);
        REP(i,h) REP(j,w) if(map[i][j] == 'x') broken[i] += (1<<j);
        printf("Case #%d: %d\n", cc+1, go(0, 0));
    }
}

