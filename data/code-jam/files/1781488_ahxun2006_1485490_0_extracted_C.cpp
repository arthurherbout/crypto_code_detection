#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
using namespace std;

int N, M;
int A[105], B[105];
long long a[105], b[105];
long long ways[105][105];

long long solve(int n, int m) {
    if (n == 0 || m == 0) return 0;
    long long &res= ways[n][m];
    if (res >= 0) return res;
    if (A[n - 1] == B[m - 1]) {
        long long sumI = 0;
        for (int i = n - 1; i >= 0; --i) {
            if (A[i] != A[n - 1]) continue;
            sumI += a[i];
            long long sumJ = 0;
            for (int j = m - 1; j >= 0; --j) {
                if (B[j] != B[m - 1]) continue;
                sumJ += b[j];
                res = max(res, min(sumI, sumJ) + solve(i, j));
            }    
        }    
    } else {
        res = max(res, solve(n - 1, m));
        res = max(res, solve(n, m - 1));
    }        
    return res;
}
    
int main() {
    freopen("C-small-attempt0.in", "r", stdin);
    freopen("C-small-attempt0.out", "w", stdout);
    int T;
    scanf("%d", &T);
    for (int caseId = 1; caseId <= T; ++caseId) {
        scanf("%d %d", &N, &M);
        for (int i = 0; i < N; ++i) {
            scanf("%I64d %d", &a[i], &A[i]);
        }    
        for (int i = 0; i < M; ++i) {
            scanf("%I64d %d", &b[i], &B[i]);
        }    
        memset(ways, -1, sizeof(ways));
        long long res = solve(N, M);
        printf("Case #%d: %I64d\n", caseId, res);
    }    
    //while (1);
    //system("pause");
    return 0;
}    
