#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

#define Rep(i,n) for(int i=0;i<(n);++i)
#define Repd(i,n) for(int i=((int)(n))-1;i>=0;--i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)
#define fi first
#define se second
#define pb push_back
#define MP make_pair

typedef pair<int,int> PII;
typedef vector<int> VI;

#define debug cout << "Here " << __LINE__ << endl;
template <typename T> ostream& operator<<(ostream &os, vector<T> v) { Rep(i, v.size()) os << v[i] << " "; os << endl; return os; }
template <typename T1, typename T2> ostream& operator<<(ostream &os, pair<T1, T2> p) { os << "(" << p.fi << ", " << p.se << ")"; return os; }

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MAX 222
 
struct Graph {
    int N;
    vector< vector<int> > ke;
    vector<bool> gt, dt;
    bool satisfy;
    // queue
    int q[MAX*2+10], le, ri;
    bool inq[MAX*2+10];
    
    void init(int n) {
        N = n;
        ke.resize(N+1);
        gt.resize(N+1);
        dt.resize(N+1);
        satisfy = true;
    }
    
    void add_edge( int a, int b) {
        ++a; ++b;
        ke[a].push_back(b);
    }
    
    int doi(int x) {
        return N+1 - x;
    }
    
    void process();
    bool gan(int);
    
};
 
bool Graph::gan(int i) {
    memset( inq, 0, sizeof(inq));
    q[le=ri=0] = i;
    inq[i] = true;
    while(le<=ri) {
        int u = q[le++];
        for(int k=0;k<ke[u].size();++k) {
            int v = ke[u][k];
            if(dt[v] && !gt[v]) return false;
            if(v==doi(i)) return false;
            if((!inq[v]) && !dt[v]) {
                q[++ri] = v;
                inq[v] = true;
            }
        }
    }
    for(int k=0;k<=ri;++k) {
        int u = q[k];
        dt[u] = dt[doi(u)] = true;
        gt[u] = true;
        gt[doi(u)] = false;
    }
    return true;
}
 
void Graph::process() {
    for(int i=1;i<=N/2;++i) if( (!dt[i]) && !dt[doi(i)] ) {     
        if( !gan(i) ) {
            if(!gan(doi(i))) {
                satisfy = false;
                return;
            }
        }       
    }
}
 
////////////////////////////////////////////////////////////////////////

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

int m, n;
string a[55];
string b[55];
int beamID[55][55];
int nbeam;
bool canVertical[111], canHorizontal[111];

// bool imply[222][222];

int value[111];

PII add(PII p, int d) {
    return MP(p.fi + dx[d], p.se + dy[d]);
}

int nextd(int d, char c) {
    if (c == '\\') return 3 - d;
    else if (c == '/') {
        return (d == 0 || d == 1) ? (1 - d) : (5 - d);
    } else return -1;
}

bool cantEnter(PII p) {
    return p.fi < 0 || p.fi >= m || p.se < 0 || p.se >= n || a[p.fi][p.se] == '#';
}

bool visited[55][55];

bool check(int i, int j, int d) {
    vector<PII> q; q.pb(MP(i, j));
    memset(visited, false, sizeof(visited));
    for (int k = 0; k < q.size(); ++k) {
        PII u = q[k];
        char c = a[u.fi][u.se];
        visited[u.fi][u.se] = true;
        if (c == '\\' || c == '/') {
            d = nextd(d, c);
        }
        PII v = add(u, d);
        if (cantEnter(v)) {}
        else {
            if (!visited[v.fi][v.se]) q.pb(v);
            if (a[v.fi][v.se] == '-' || a[v.fi][v.se] == '|') {
                return false;
            }
        }
    }
    return true;
}

int take(int i, int j) {
    if (i == -1) {
        return j;
    }
    else {
        // if (j != -1) cout << i << " " << j << endl;
        return i;
    }
}

int get(int i, int j, int d) {
    // cout << "=========================" << endl;
    vector<PII> q; q.pb(MP(i, j));
    memset(visited, false, sizeof(visited));
    for (int k = 0; k < q.size(); ++k) {
        PII u = q[k];
        // cout << u << " " << d << endl;
        char c = a[u.fi][u.se];
        visited[u.fi][u.se] = true;
        if (c == '\\' || c == '/') {
            d = nextd(d, c);
        }
        PII v = add(u, d);
        if (cantEnter(v)) {}
        else {
            if (!visited[v.fi][v.se]) q.pb(v);
            if (a[v.fi][v.se] == '-' || a[v.fi][v.se] == '|') {
                return (d == 1 || d == 3) ? beamID[v.fi][v.se] : (2 * nbeam - 1 - beamID[v.fi][v.se]);
            }
        }
    }
    return -1;
}

int main() {
    int nt;
    cin >> nt;
    Rep(t, nt) {
        cin >> m >> n;
        Rep(i, m) cin >> a[i];
        nbeam = 0;
        memset(beamID, -1, sizeof(beamID));
        Rep(i, m) Rep(j, n) if (a[i][j] == '-' || a[i][j] == '|') {
            beamID[i][j] = nbeam;
            nbeam++;
        }
        Rep(i, m) Rep(j, n) if (beamID[i][j] != -1) {
            canVertical[beamID[i][j]] = check(i, j, 1) && check(i, j, 3);
            canHorizontal[beamID[i][j]] = check(i, j, 0) && check(i, j, 2);
        }

        Graph g; g.init(nbeam * 2);

        bool ok = true;
        Rep(i, nbeam) if (!canVertical[i] && !canHorizontal[i]) ok = false;
        if (ok) {
            Rep(i, nbeam) {
                if (!canVertical[i]) g.add_edge(i, 2 * nbeam - i - 1); // imply[i][i + nbeam] = true;
                if (!canHorizontal[i]) g.add_edge(2 * nbeam - i - 1, i); // imply[i + nbeam][i] = true;
            }
            Rep(i, m) Rep(j, n) if (a[i][j] == '.') {
                // cout << get(i, j, 1) << endl;
                int beam1 = take(get(i, j, 1), get(i, j, 3));
                int beam2 = take(get(i, j, 0), get(i, j, 2));
                // cout << beam1 << " " << beam2 << endl;
                if (beam1 != -1 && beam2 != -1) {
                    // imply[beam1 + nbeam][beam2] = true;
                    // imply[beam2 + nbeam][beam1] = true;
                    g.add_edge(2 * nbeam - beam1 - 1, beam2);
                    g.add_edge(2 * nbeam - beam2 - 1, beam1);
                } else {
                    if (beam1 != -1) {
                        // imply[beam1 + nbeam][beam1] = true;
                        g.add_edge(2 * nbeam - beam1 - 1, beam1);
                    }
                    else if (beam2 != -1) {
                        // imply[beam2 + nbeam][beam2] = true;   
                        g.add_edge(2 * nbeam - beam2 - 1, beam2);
                    }
                    else {
                        ok = false;
                    }
                }
            }
            // Rep(k, 2 *nbeam) Rep(i, 2 * nbeam) Rep(j, 2 * nbeam)
            //     if (imply[i][k] && imply[k][j]) imply[i][j] = true;

            if (ok) {
                g.process();
                if (!g.satisfy) {
                    ok = false;
                } else {
                    Rep(i, m) b[i] = a[i];
                    Rep(i, m) Rep(j, n) if (beamID[i][j] != -1) {
                        int id = beamID[i][j];
                        if (g.gt[id+1]) b[i][j] = '-';
                        else b[i][j] = '|';
                    }
                }
            }
        }

        cout << "Case #" << (t + 1) << ": ";
        if (ok) {
            cout << "POSSIBLE" << endl;
            Rep(i, m) cout << b[i] << endl;
        } else {
            cout << "IMPOSSIBLE" << endl;
        }
    }
    return 0;
}
