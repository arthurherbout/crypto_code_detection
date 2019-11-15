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

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "start test case %d\n", ti);
        int n, p; scanf("%d%d", &n, &p);
        vector<int> req(n);
        for(int i=0;i<n;i++) scanf("%d", &req[i]);
        vector<vector<int>> quan(n, vector<int>(p));
        for(int i=0;i<n;i++){
            for(int j=0;j<p;j++) scanf("%d", &quan[i][j]);
        }
        for(int i=0;i<n;i++){
            sort(quan[i].begin(), quan[i].end());
        }
        vector<vector<pii>> x(n);
        for(int i=0;i<n;i++){
            for(int j=0;j<p;j++){
                int lo = f_lo(req[i], quan[i][j]);
                int hi = f_hi(req[i], quan[i][j]);
                if(lo <= hi){
                    x[i].emplace_back(lo, hi);
                }
            }
        }
        if(n == 1){
            printf("Case #%d: %d\n", ti, (int)x[0].size());
        }
        else{
            int cnt = 0;
            vector<int> vis(p, 0);
            for(int i=0;i<x[0].size();i++){
                for(int j=0;j<x[1].size();j++){
                    if(vis[j]) continue;
                    int l1 = x[0][i].first, r1 = x[0][i].second;
                    int l2 = x[1][j].first, r2 = x[1][j].second;
                    if(max(l1,l2) <= min(r1,r2)){
                        vis[j] = true;
                        cnt++;
                        break;
                    }
                }
            }
            printf("Case #%d: %d\n", ti, cnt);
        }
    }
    return 0;
}
