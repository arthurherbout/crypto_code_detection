#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define rep(i,b) for(auto i=(0);i<(b);++i)
#define fo(i,a,b) for(auto i=(a);i<=(b);++i)
#define ford(i,a,b) for(auto i=(a);i>=(b);--i)
#define fore(a,b) for(auto a = (b).begin();a!=(b).end();++a)
#define vv vector
#define pb push_back
#define ll long long
#define ld long double
#define ss(a) (int)(a).size()
#define all(x) (x).begin(),(x).end()
#define clr(x,a) memset(x,a,sizeof(x))
#define vi vv<int>
#define vs vv<string>
#define i2 array<int,2>
#define i3 array<int,3>

int cond = (ll)1;
#define db(x) { if (cond > 0) { cond--; rep (xxx, 1) cerr << __LINE__ << " " << #x << " " << x << endl; cerr.flush(); } }

char A[111][111];
int dr[]={-1,1,0,0};
int dc[]={0,0,-1,1};
vector<int>nei[111][111];
int vis[111][111];
#define MOD 103

bool VV[MOD] = {};

struct solver {
    void solve() {
        // function<int (int)> dfs=[&](auto x) { return 0; };
        // auto comp=[&](auto x, auto y){ return 0; };
        int C,R,M;
        cin>>C>>R>>M;
        fo(r,0,R+1)fo(c,0,C+1)A[r][c]='#';
        fo(r,1,R)fo(c,1,C)nei[r][c].clear();
        fo(r,1,R)fo(c,1,C)cin>>A[r][c];
        int turnum=0;
        fo(r,1,R)fo(c,1,C)if(A[r][c]=='T') {
            turnum++;
            db(r<<" "<<c);
            rep(d,4) fo(k,0,1000) {
                int nr=r+dr[d]*k;
                int nc=c+dc[d]*k;
                if(A[nr][nc]=='#') break;
                nei[nr][nc].push_back(turnum);
            }
        }
        vector<vector<int> >nnn;
        vector<int>match[MOD*MOD];
        fo(r,1,R)fo(c,1,C)if(A[r][c]=='S') {
            fo(r,0,R+1)fo(c,0,C+1)vis[r][c]=-1;
            vis[r][c]=M;
            vector<pair<int,int> > Q; Q.push_back(pair<int,int>(r,c));
            set<int>found;
            for(int i =0;i<Q.size();i++) {
                int pr=Q[i].first;int pc=Q[i].second;
                fore(it,nei[pr][pc])found.insert(*it);
                if(vis[pr][pc]==0)continue;
                rep(d,4) {
                    int nr=pr+dr[d];int nc=pc+dc[d];
                    if(A[nr][nc]=='#')continue;
                    if(vis[nr][nc] == -1) {
                        vis[nr][nc]=vis[pr][pc]-1;
                        Q.push_back(pair<int,int>(nr,nc));
                    }
                }
            }
            db("");
            nnn.push_back(vector<int>());
            fore(it,found)nnn.back().push_back(*it);
        }
        int S=nnn.size();
        db(S);
        int skoj[MOD*MOD]={};
        rep(i,MOD*MOD)skoj[i]=-1;
        vector<pair<int,int> > res;
        rep(i,S) {
            function<int (int)> dfs=[&](int p) {
                if (VV[p]) return 0;
                VV[p] = 1;
                fore (it, nnn[p]) {
                    if (skoj[*it]==-1) {
                        skoj[*it] = p;
                        return 1;
                    }
                    if (dfs(skoj[*it])) {
                        skoj[*it] = p;
                        return 1;
                    }
                }
                return 0;
            };
            rep(j, MOD)VV[j]=0;
            fore(it,nnn[i]) {
                if (skoj[*it]==-1) {
                    skoj[*it]=i;
                    break;
                } else {
                    if (dfs(skoj[*it])) {
                        skoj[*it]=i;
                        break;
                    }
                }
            }
        }
            fo(t,1,turnum)if(skoj[t]!=-1) {
                res.push_back(pair<int,int>(skoj[t]+1, t));
            }
        cout << res.size() << endl;
        fore(it,res) cout<<it->first<<" "<<it->second<<endl;
    }
};

const int multi = 0;
const int gcj = 1;

int main(int argc, char ** argv) {
    ios::sync_with_stdio(false); cond = argc >= 2 && argv[1][0] == 'q' ? 1 << 30 : 0;
    std::cout.setf(ios::fixed);
    std::cout.precision(10);
    // auto f = [&](){return 0;};

    int t;
    if (multi | gcj)
        cin >> t;
    else
        t = 1;

    fo (i, 1, t) {
        if (cond) cerr << __LINE__ << " " << i << endl;
        if (gcj) cout << "Case #" << i << ": ";
        solver s;
        s.solve();
    }
	return 0;
}
