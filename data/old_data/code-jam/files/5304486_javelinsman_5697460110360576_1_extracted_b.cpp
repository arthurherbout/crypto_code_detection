#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef pair<int,int> pii;

int f_lo(int r, int q){
    if((10 * q) % (11 * r) == 0) return (10*q)/(11*r);
    else return (10*q)/(11*r) + 1;
}
int f_hi(int r, int q){
    return (10 * q) / (9 * r);
}

int n, p;
vector<vector<pii>> x;
vector<vector<int>> vis;

int dfs(int i, int j){
    vis[i][j] = true;
    if(i == n-1) return 1;
    int l1 = x[i][j].first, r1 = x[i][j].second;
    for(int k=0;k<x[i+1].size();k++){
        int l2 = x[i+1][k].first, r2 = x[i+1][k].second;
        if(max(l1,l2) <= min(r1,r2) && !vis[i+1][k]){
            if(dfs(i+1, k)) return 1;
        }
    }
    return 0;
}

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "start test case %d\n", ti);
        scanf("%d%d", &n, &p);
        vector<int> req(n);
        for(int i=0;i<n;i++) scanf("%d", &req[i]);
        vector<vector<int>> quan(n, vector<int>(p));
        for(int i=0;i<n;i++){
            for(int j=0;j<p;j++) scanf("%d", &quan[i][j]);
        }
        for(int i=0;i<n;i++){
            sort(quan[i].begin(), quan[i].end());
        }
        x = vector<vector<pii>>(n);
        vis = vector<vector<int>>(n, vector<int>(p, 0));
        for(int i=0;i<n;i++){
            for(int j=0;j<p;j++){
                int lo = f_lo(req[i], quan[i][j]);
                int hi = f_hi(req[i], quan[i][j]);
                if(lo <= hi){
                    x[i].emplace_back(lo, hi);
                }
            }
        }
        int cnt = 0;
        for(int i=0;i<x[0].size();i++){
            if(dfs(0, i)) cnt++;
        }
        printf("Case #%d: %d\n", ti, cnt);
    }
    return 0;
}
