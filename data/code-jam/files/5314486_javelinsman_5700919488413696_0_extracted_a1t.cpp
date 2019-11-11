#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;


int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "testcase %d\n", ti);
        int n, p; scanf("%d%d", &n, &p);
        vector<int> g(n); for(int i=0;i<n;i++) scanf("%d", &g[i]);
        int cnt[4] = {};
        for(int i=0;i<n;i++) cnt[g[i]%p]++;
        if(p == 2){
            int ans = cnt[0] + (cnt[1] + 1)/2;
            printf("Case #%d: %d\n", ti, ans);
        }
        if(p == 3){
            int ans = cnt[0];
            int a = max(cnt[1], cnt[2]), b = min(cnt[1], cnt[2]);
            ans += b; a -= b;
            ans += (a+2)/3;
            printf("Case #%d: %d\n", ti, ans);
        }
    }
    return 0;
}
