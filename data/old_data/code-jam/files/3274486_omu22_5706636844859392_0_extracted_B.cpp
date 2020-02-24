#include <bits/stdc++.h>

#define rep(i, n) for(int i = 0;i < n;i++)
#define REP(i, a, n) for(int i = a;i < n;i++)
#define all(i) i.begin(), i.end()


using namespace std;

typedef pair<int, int> P;
double PI = acos(-1);

const int M = 24 * 60 + 1;
const int R = M / 2 + 1;

int dp[M + 2][R + 2][2][2];
int cf[M + 2];
int jf[M + 2];

int main(){

    int T;
    cin >> T;

    rep(_, T){
        cout << "Case #" << _ + 1 << ": ";

        rep(i, M+2)rep(j, R+2)rep(k,2)rep(l,2)dp[i][j][k][l] = 1e9;
        rep(i, M+2) cf[i] = 0;
        rep(i, M+2) jf[i] = 0;

        int ac, aj;
        cin >> ac >> aj;

        rep(i, ac){
            int st, ed;
            cin >> st >> ed;
            REP(j, st, ed){
                cf[j] = 1;
            }
        }

        rep(i, aj){
            int st, ed;
            cin >> st >> ed;
            REP(j, st, ed){
                jf[j] = 1;
            }
        }

        dp[0][0][0][0] = dp[0][0][1][1] = 0; 

        rep(t, 1440){
            rep(c, 721){
                rep(i, 2){
                    rep(j, 2){
                        //i == 0 c
                        //i == 1 j
                        if(cf[t] == 0){
                            dp[t + 1][c + 1][0][j] = min(dp[t + 1][c + 1][0][j], dp[t][c][i][j] + (i ? 1 : 0));
                        }
                        if(jf[t] == 0){
                            dp[t + 1][c + 0][1][j] = min(dp[t + 1][c + 0][1][j], dp[t][c][i][j] + (i ? 0 : 1));
                        }
                    }
               }
            }
        }
        int ans = 1e9;
        ans = min(ans, dp[1440][720][0][0]);
        ans = min(ans, dp[1440][720][0][1] + 1);
        ans = min(ans, dp[1440][720][1][1]);
        ans = min(ans, dp[1440][720][1][0] + 1);

        cout << ans << endl;


    }



    return 0;
}