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

const int M = 10007;

bool rook[101][101];
int c[101][101];

int main()
{    
    int cases;
    cin >> cases;
    REP(cc,cases)
    {
        int h, w, r;
        cin >> h >> w >> r;
        CLEAR(rook,0); CLEAR(c,0);
        c[0][0] = 1;
        REP(i,r)
        {
            int y, x;
            cin >> y >> x;
            --y; --x;
            rook[y][x] = true;
        }
        REP(i,h) REP(j,w) if(!rook[i][j])
        {
            if(i >= 2 && j >= 1) c[i][j] += c[i-2][j-1];
            if(i >= 1 && j >= 2) c[i][j] += c[i-1][j-2];
            c[i][j] %= M;
        }
        printf("Case #%d: %d\n", cc+1, c[h-1][w-1]);
    }
}

