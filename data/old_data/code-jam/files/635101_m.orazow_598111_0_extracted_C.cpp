#include<cstdio>
#include<cstring>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

#define FOR(i, a, b) for (int i(a), _b(b); i <= _b; i++)
#define FORD(i, a, b) for (int i(a), _b(b); i >= _b; i--)
#define REP(i, n) for (int i(0), _n(n); i < _n; i++)
#define REPD(i, n) for (int i((n)-1); i >= 0; i--)
#define CLR(x, with) memset((x), with, sizeof(x))
#define ALL(x) (x).begin(), (x).end()

typedef long long LL;
typedef unsigned long long ULL;

#define MOD 100003

int main() {
    int T, n;
    scanf("%d", &T);
    FOR(t, 1, T)
    {
        printf("Case #%d: ", t);
        scanf("%d", &n);
        int sum = 2;
        FOR(i, 3, n) sum = (sum + (n-i)) % MOD;
        printf("%d\n", sum);
    }

    return 0;
}
