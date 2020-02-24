#include <cstdio>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

const int INF = 987654321;

int cache2[2][1][111];
int cache3[3][1][111][111];
int cache4[4][1][111][111][111];

int dp2(int offset, int c0, int c1){
    int& ret = cache2[offset][c0][c1];
    if(ret != -1) return ret;
    ret = 0;
    if(c0 + c1 == 1) return ret = 0;
    if(c0 > 0) ret = max(ret, (offset == 0) + dp2(offset, c0-1, c1));
    if(c1 > 0) ret = max(ret, ((offset+1)%2==0) + dp2((offset+1)%2, c0, c1-1));
    return ret;
}

int dp3(int offset, int c0, int c1, int c2){
    int& ret = cache3[offset][c0][c1][c2];
    if(ret != -1) return ret;
    ret = 0;
    if(c0 + c1 + c2 == 1) return ret = 0;
    if(c0 > 0) ret = max(ret, (offset == 0) + dp3(offset, c0-1, c1, c2));
    if(c1 > 0) ret = max(ret, ((offset+1)%3==0) + dp3((offset+1)%3, c0, c1-1, c2));
    if(c2 > 0) ret = max(ret, ((offset+2)%3==0) + dp3((offset+2)%3, c0, c1, c2-1));
    //printf("dp(%d, %d, %d, %d) = %d\n", offset, c0, c1, c2, ret);
    return ret;
}

int dp4(int offset, int c0, int c1, int c2, int c3){
    int& ret = cache4[offset][c0][c1][c2][c3];
    if(ret != -1) return ret;
    ret = 0;
    if(c0 + c1 + c2 + c3 == 1) return ret = 0;
    if(c0 > 0) ret = max(ret, (offset == 0) + dp4(offset, c0-1, c1, c2, c3));
    if(c1 > 0) ret = max(ret, ((offset+1)%4==0) + dp4((offset+1)%4, c0, c1-1, c2, c3));
    if(c2 > 0) ret = max(ret, ((offset+2)%4==0) + dp4((offset+2)%4, c0, c1, c2-1, c3));
    if(c3 > 0) ret = max(ret, ((offset+3)%4==0) + dp4((offset+3)%4, c0, c1, c2, c3-1));
    //printf("dp(%d, %d, %d, %d) = %d\n", offset, c0, c1, c2, ret);
    return ret;
}


int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "testcase %d\n", ti);
        memset(cache2, -1, sizeof(cache2));
        memset(cache3, -1, sizeof(cache3));
        memset(cache4, -1, sizeof(cache4));
        int n, p; scanf("%d%d", &n, &p);
        vector<int> g(n); for(int i=0;i<n;i++) scanf("%d", &g[i]);
        int cnt[4] = {};
        for(int i=0;i<n;i++) cnt[g[i]%p]++;
        if(p == 2){
            int toadd = cnt[0];
            if(cnt[0] == n) toadd--;
            printf("Case #%d: %d\n", ti, 1 + toadd + dp2(0, 0, cnt[1]));
        }
        if(p == 3){
            int toadd = cnt[0];
            if(cnt[0] == n) toadd--;
            printf("Case #%d: %d\n", ti, 1 + toadd + dp3(0, 0, cnt[1], cnt[2]));
        }
        if(p == 4){
            int toadd = cnt[0];
            if(cnt[0] == n) toadd--;
            printf("Case #%d: %d\n", ti, 1 + toadd + dp4(0, 0, cnt[1], cnt[2], cnt[3]));
        }
    }
    return 0;
}
