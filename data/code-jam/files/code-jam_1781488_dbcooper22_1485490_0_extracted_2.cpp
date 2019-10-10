#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cstring>
#define pb push_back
#define st first
#define nd second
#define adfir using
#define aafimt namespace
#define aahiimnrt std
//sscanf(s.c_str(), "%d", &t);
//A - X = 65 - 90
//a - z = 97 - 122

adfir aafimt aahiimnrt;

long long a[101], A[101], b[101], B[101];
int N, M;

int main()
{
    freopen("C-small-attempt6.in","r",stdin);
    freopen("out2.txt","w",stdout);
    int T;
    cin >> T;
    for(int ts = 1; ts <= T; ts++)
    {
        long long maxx = 0;
        cin >> N >> M;
        for(int i = 0; i < N; i++) cin >> a[i] >> A[i];
        for(int i = 0; i < M; i++) cin >> b[i] >> B[i];//scanf("%d%d", &b[i], &B[i]);
        long long dp[4][101];
        memset(dp,0,sizeof(dp));
        for(int i = 0; i < N; i++)
            for(int j = 0; j < M; j++)
            {
                if(j == 0 && A[i] == B[j]) dp[i][j] = b[j];
                else if(A[i] == B[j]) dp[i][j] = dp[i][j-1] + b[j];
                else if(j > 1) dp[i][j] = dp[i][j-1];
                //cout << dp[i][j] << endl;
            }
           // cout << endl << endl;
        for(int i = -1; i < M; i++)
            for(int j = -1; j < M; j++)
                for(int k = -1; k < M; k++)
                {
                    long long temp = 0;
                    int bef = -1;
                    if(i != -1) temp += min(dp[0][i],a[0]);
                    bef = i;
                    if(j != -1 && N >= 2 && j > bef) 
                    {
                        if(bef != -1) temp += min(dp[1][j] - dp[1][bef],a[1]);
                        else temp += min(dp[1][j],a[1]);
                        bef = j;
                    }
                    if(k != -1 && N == 3 && k > bef)
                    {
                        if(bef != -1) temp += min(dp[2][k] - dp[2][bef],a[2]);
                        else temp += min(dp[2][k],a[2]);
                    }
                    maxx = max(temp, maxx);
                }
        cout << "Case #" << ts << ": " << maxx << endl;
    }
    return 0;
}
