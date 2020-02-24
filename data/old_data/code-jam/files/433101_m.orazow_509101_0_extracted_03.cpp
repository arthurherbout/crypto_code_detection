#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<queue>
#include<algorithm>
using namespace std;

#define FOR(i, a, b) for (int i(a), _b(b); i <= _b; i++)
#define FORD(i, a, b) for (int i(a), _b(b); i >= _b; i--)
#define REP(i, n) for (int i(0), _n(n); i < _n; i++)
#define REPD(i, n) for (int i((n)-1); i >= 0; i--)
#define CLR(x, with) memset((x), with, sizeof(x))
#define ALL(x) (x).begin(), (x).end()

template<typename T> inline void getMax(T& a, T b) { if (b > a) a = b; }
template<typename T> inline void getMin(T& a, T b) { if (b < a) a = b; }

typedef long long LL;
typedef unsigned long long ULL;

int R, K, N;
int G[1001];


int main() {
    int tcases;
    ULL res, sum;
    int n;

    scanf("%d", &tcases);

    FOR(t, 1, tcases)
    {
        scanf("%d %d %d", &R, &K, &N);
        queue<int> Q, temp;
        REP(i, N) {scanf("%d", &G[i]); Q.push(G[i]); }

        res = 0;
        REP(i, R)
        {
            sum = 0;
            while (1)
            {
                if (Q.empty()) break;
                n = Q.front();
                if (sum + n <= K)
                {
                    Q.pop(); sum += n; temp.push(n);
                }
                else break;
            }
            while (!temp.empty()) { n = temp.front(); Q.push(n); temp.pop(); }
            res += sum;
        }

        printf("Case #%d: %d\n", t, res);
    }

    return 0;
}
