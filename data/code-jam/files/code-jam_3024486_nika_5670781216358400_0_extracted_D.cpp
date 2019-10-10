#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <cstring>
#include <queue>
#include <sstream>
using namespace std;
typedef long long ll;
typedef pair<int,int> pii;
#define SZ(x) (int)(x.size())
#define F0(i,n) for(int i=0;i<n;i++)
#define F1(i,n) for(int i=1;i<=n;i++)
#define CL(a,x) memset(x, a, sizeof(x))
const int inf = 1000000009;
const double pi = atan(1.0)*4.0;
const double eps = 1e-8;
ll gcd(ll x, ll y) { return y ? gcd(y, x%y) : x; }
int bc(int n) { return n ? bc((n-1)&n)+1 : 0; }

int i, j, k, m, n, l, ans;
char ss[1000002];

int d[5005];
vector<int> adj[5005];

bool cont[100][100][100];
bool conte[81][81][81][81];

vector<int> v;

void dfs(int i, int start, int pr) {
    F0(j,SZ(v)) {
        cont[start][i][v[j]] = 1;
        if (j>0) {
            conte[start][i][v[j-1]][v[j]] = 1;
            conte[start][i][v[j]][v[j-1]] = 1;
        }
    }

    F0(j,SZ(adj[i])) if (adj[i][j] != pr) {
        v.push_back(adj[i][j]);
        dfs(adj[i][j], start, i);
        v.pop_back();
    }
}


int rec(int i, int j, int i0, int j0) {

    bool has = false;
    F0(k,SZ(adj[j])) {
        int l = adj[j][k];
        if (!conte[i0][i][j][l] && !conte[j0][j][j][l]) has = true;
    }

    int best = -inf;

    F0(k,SZ(adj[i])) {
        int l = adj[i][k];
        if (!conte[i0][i][i][l] && !conte[j0][j][i][l]); else continue;

        int add = 0;
        if (!cont[i0][i][l] && !cont[j0][j][l]) add = d[l];
        best = max(best, add-rec(j,l,j0,i0));
    }
    if (best == -inf) {
        if (!has) best = 0;
        else best = -rec(j,i,j0,i0);
    }
   // cout << i << " " << j << " " << i0 << " " << j0 << " " << best << endl;
    return best;
}

int sol(int i, int j) {
    return rec(i, j, i, j);
}

int main() {
//    freopen("x.in", "r", stdin);

    freopen("D-small-attempt0.in", "r", stdin);
    freopen("D-small-attempt0.out", "w", stdout);

//	freopen("D-large.in", "r", stdin);
//	freopen("D-large.out", "w", stdout);

	int tt, tn; cin >> tn;
	F1(tt,tn) {
        cerr << tt << endl;
        cin >> n;
        F1(i,n) adj[i].clear();

        F1(i,n) {
            cin >> d[i];
        }
        F1(i,n-1) {
            cin >> j;
            adj[i].push_back(j);
            adj[j].push_back(i);
        }

        int ans = -inf;

        CL(0,cont);
        CL(0,conte);

        F1(i,n) {
            v.clear();
            v.push_back(i);
            dfs(i, i,-1);
        }

        F1(i,n) {
            int best = inf;

            F1(j,n) if (i != j) {
                best = min(best, sol(i,j)+d[i]-d[j]);
            }
            ans = max(ans, best);
        }

        printf("Case #%d: %d\n", tt, ans);
	}

	return 0;
}
