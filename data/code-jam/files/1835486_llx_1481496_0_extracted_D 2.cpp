#include <cmath>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

const char SP[] = "oi2eas6tbg";

char S[5050];

bool yes[333][333];
int flag1[333], flag2[333];
int tail[333], head[333];

int ddfs(int ix, int jx) {
    yes[ix][jx] = false;
    for (int i = '0'; i <= '9'; ++i) {
        if (yes[jx][i]) {
            return (1+ddfs(jx, i));
        }
    }
    for (int i = 'a'; i <= 'z'; ++i) {
        if (yes[jx][i]) {
            return (1+ddfs(jx, i));
        }
    }
    if (head[jx] > 0) {
        --head[jx];
        return 0;
    }
    ++tail[jx];
    return 1;
}

int dfs(int ix, int jx) {
    int res = ddfs(ix, jx);
    ++head[ix];
    return (res+1);
}

int main() {
    FILE *fin, *fout;    
    fin = fopen("D:\\TopCoder\\gcj2012\\R3\\D-small-attempt0.in", "r");
    fout = fopen("D:\\TopCoder\\gcj2012\\R3\\D.out", "w");
    int T;
    fscanf(fin, "%d\n", &T);
    for (int ca = 1; ca <= T; ca++) {
        int K;
        fscanf(fin, "%d\n", &K);
        fscanf(fin, "%s", S);
        memset(yes, false, sizeof(yes));
        memset(head, 0, sizeof(head));
        memset(tail, 0, sizeof(tail));
        memset(flag1, 0, sizeof(flag1));
        memset(flag2, 0, sizeof(flag2));

        int len = strlen(S);
        for (int i = 0; i+1 < len; ++i) {
            int k = i+1;
            {
                yes[ S[i] ][ S[k] ] = true;
                bool yes1 = false, yes2 = false;
                char C1, C2;
                for (int j = 0; j <= 9; ++j) {
                    if (S[i] == SP[j]) {
                        yes[ '0'+j ][ S[k] ] = true;
                        yes1 = true;
                        C1 = '0'+j;
                    }
                }
                for (int j = 0; j <= 9; ++j) {
                    if (S[k] == SP[j]) {
                        yes[ S[i] ][ '0'+j ] = true;
                        yes2 = true;
                        C2 = '0'+j;
                    }
                }
                if (yes1 && yes2) {
                    yes[ C1 ][ C2 ] = true;
                }
            }
        }
        
        int ans = 0;
        for (int i = 'a'; i <= 'z'; ++i) {
            for (int j = 'a'; j <= 'z'; ++j) {
                if (yes[i][j]) {
                    ans += dfs(i, j);
                    ++flag1[i];
                    ++flag2[j];
                }
            }
        }
        for (int i = '0'; i <= '9'; ++i) {
            for (int j = 'a'; j <= 'z'; ++j) {
                if (yes[i][j]) {
                    ans += dfs(i, j);
                    ++flag1[i];
                    ++flag2[j];
                }
            }
        }
        for (int i = 'a'; i <= 'z'; ++i) {
            for (int j = '0'; j <= '9'; ++j) {
                if (yes[i][j]) {
                    ans += dfs(i, j);
                    ++flag1[i];
                    ++flag2[j];
                }
            }
        }
        for (int i = '0'; i <= '9'; ++i) {
            for (int j = '0'; j <= '9'; ++j) {
                if (yes[i][j]) {
                    ans += dfs(i, j);
                    ++flag1[i];
                    ++flag2[j];
                }
            }
        }
        int cnt = 0, cnt1 = 0, cnt2 = 0;
        for (int i = '0'; i <= '9'; ++i) {
            cnt2 += min(head[i], tail[i]);
            cnt1 += max(head[i], tail[i]);
            if (flag1[i] && flag2[i]) cnt += min(flag1[i], flag2[i]);
        } 
        for (int i = 'a'; i <= 'z'; ++i) {
            cnt2 += min(head[i], tail[i]);
            cnt1 += max(head[i], tail[i]);
            if (flag1[i] && flag2[i]) cnt += min(flag1[i], flag2[i]);
        }
        cout << "==" << ans << " " << cnt2 << " " << cnt1 << endl;///
        ans = max(ans-cnt2, ans-cnt1+1);
        cout << ">>==" << ans << " " << cnt2 << " " << cnt1 << endl;///
        fprintf(fout, "Case #%d: %d\n", ca, ans);
    }
    
    fclose(fin);
    fclose(fout);
    return 0;
}



