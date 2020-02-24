#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define foreach(iter, cont) \
    for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)
typedef long long LL;

bool compatible(int k, int* a, int* b)
{
    for (int i = 1; i < k; i++) {
        if (a[i-1] >= b[i-1] && a[i] <= b[i])
            return false;
        if (b[i-1] >= a[i-1] && b[i] <= a[i])
            return false;
    }
    return true;
}

bool adj[100][100];
int used[100];
int n, k, unused, best;

void bruteforce(int level, int start)
{
    if (unused == 0) {
        best = min(best, level);
        return;
    }
    if (level >= best)
        return;

    if (start == 0) {
        int q = 0;
        while (used[q] != -1) q++;
        used[q] = level;
        unused--;
        bruteforce(level, q+1);
        unused++;
        used[q] = -1;
        return;
    }

    if (start == n) {
        bruteforce(level+1, 0);
        return;
    }

    if (used[start] != -1) {
        bruteforce(level, start+1);
        return;
    }

    bool good = true;
    for (int j = 0; j < start; j++) {
        if (used[j] == level && !adj[j][start]) {
            good = false;
            break;
        }
    }

    if (good) {
        used[start] = level;
        unused--;
        bruteforce(level, start+1);
        unused++;
        used[start] = -1;
    }
    bruteforce(level, start+1);
}

int solve()
{
    scanf("%d %d", &n, &k);
    int* chart[n];
    for (int i = 0; i < n; i++) {
        chart[i] = new int[k];
        for (int j = 0; j < k; j++)
            scanf("%d", &chart[i][j]);
    }

    for (int i = 0; i < n; i++)
    for (int j = i+1; j < n; j++) {
        adj[i][j] = compatible(k, chart[i], chart[j]);
        adj[j][i] = adj[i][j];
    }

    best = n;
    unused = n;
    for (int i = 0; i < n; i++)
        used[i] = -1;
    bruteforce(1, 0);
    return best;
}

int main()
{
    int T;
    scanf("%d", &T);
    for (int t = 1; t <= T; t++)
        printf("Case #%d: %d\n", t, solve());
    return 0;
}
