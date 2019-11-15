#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <queue>
#include <cstring>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(i=0;i<n;i++)
#define F1(i,n) for(i=1;i<=n;i++)
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }
typedef pair<int, int> pii;
int i, j, d, k, m, n, l, ans, nc, f, v[100][100];
string s[100];
set<pii> S;

int dx[]={1,0,0};
int dy[]={0,-1,1};

void dfs(int x, int y)
{
    if ((s[x][y] == '#') || v[x][y]) return;
    v[x][y] = 1;
    nc++;
    S.insert(pii(x,y));
    dfs(x,y-1);
    dfs(x,y+1);
    dfs(x-1,y);
}

set< set<pii> > SS;

bool go(set<pii> S)
{
    if (S.size() == 1) return true;
    if (SS.count(S)) return false;
    SS.insert(S);
    for (int k = 0; k < 3; k++)
    {
        int good = 1;
        set<pii> S2;
        for (set<pii>::iterator it = S.begin(); it != S.end(); it++)
        {
            int x = it->first+dx[k];
            int y = it->second+dy[k];
            if (s[x][y] == '#') { x = it->first; y = it->second; }
            else if (!v[x][y]) { good = 0; break; }
            S2.insert(pii(x,y));
        }
        if (good)
        {
            if (go(S2)) return true;
        }
    }
    return false;
}

void doit(int x, int y)
{
    nc = 0;
    f = 0;
    memset(v, 0, sizeof(v));
    S.clear();
    SS.clear();
    dfs(x,y);
    f = go(S);
}

int solve(int d)
{
    F0(i,m) F0(j,n) if (s[i][j] == d + '0')
    {
        doit(i, j);
        cout << d << ": " << nc << " " << (f ? "Lucky" : "Unlucky") << endl;
        return 1;
    }
    return 0;
}

int main() {
//	freopen("d.in", "r", stdin);

    freopen("D-small-attempt1.in", "r", stdin);
    freopen("D-small-attempt1.out", "w", stdout);

//	freopen("D-large.in", "r", stdin);
//	freopen("D-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cin >> m >> n;
        F0(i,m) cin >> s[i];
        printf("Case #%d: \n", tt);

        F0(d,10)
        {
            if (solve(d));
        }

	}
	
	return 0;
}
