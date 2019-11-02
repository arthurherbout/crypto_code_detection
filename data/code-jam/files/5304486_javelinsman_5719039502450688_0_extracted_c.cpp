#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

const int INF = 987654321;
int Hd, Ad, Hk, Ak, B, D;
int vis[111][222][111][111];
int dfs(int hd, int ad, int hk, int ak){
    int& ret = vis[hd][ad][hk][ak];
    if(ret != -1) return ret;
    ret = INF;
    // Attack
    if(hk - ad <= 0) return ret = 1;
    else{
        if(hd-ak > 0)
            ret = min(ret, 1 + dfs(hd-ak, ad, hk-ad, ak));
    }
    // Buff
    if(ad <= 100){
        if(hd-ak > 0)
            ret = min(ret, 1 + dfs(hd-ak, ad+B, hk, ak));
    }
    // Cure
    if(Hd-ak > 0)
        ret = min(ret, 1 + dfs(Hd-ak, ad, hk, ak));
    // Debuff
    if(hd-max(ak-D,0) > 0)
        ret = min(ret, 1 + dfs(hd-max(0,ak-D), ad, hk, max(0, ak-D)));
    return ret;
}

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "start test case %d\n", ti);
        scanf("%d%d%d%d%d%d", &Hd, &Ad, &Hk, &Ak, &B, &D);
        memset(vis, -1, sizeof(vis));
        int ret = dfs(Hd, Ad, Hk, Ak);
        if(ret == INF) printf("Case #%d: IMPOSSIBLE\n", ti);
        else printf("Case #%d: %d\n", ti, ret);
    }
    return 0;
}
