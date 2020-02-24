#include <climits>
#include <cstring>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

#define INF (INT_MAX / 3)
#define foreach(iter, cont) \
    for (typeof((cont).begin()) iter = (cont).begin(); iter != (cont).end(); iter++)
typedef long long LL;

int maxfall, rows, cols;
bool wall[50][50];

int dig(int x, int y, bool right, bool left)
{
    if (x == (rows-1))
        return 0;

    if (!wall[x+1][y]) {
        int nx = x+1;
        while (nx < rows && !wall[nx][y])
            nx++;
        nx--;
        if (nx - x > maxfall)
            return INF;
        return dig(nx, y, true, true);
    }

    int res = INF;
    if (right && y != (cols-1) && !wall[x][y+1])
        res = min(res, dig(x, y+1, true, false));
    if (left && y != 0 && !wall[x][y-1])
        res = min(res, dig(x, y-1, false, true));

    if (y != (cols-1) && !wall[x][y+1] && wall[x+1][y+1]) {
        wall[x+1][y+1] = false;
        res = min(res, dig(x, y, true, true) + 1);
        wall[x+1][y+1] = true;
    }
    if (y != 0 && !wall[x][y-1] && wall[x+1][y-1]) {
        wall[x+1][y-1] = false;
        res = min(res, dig(x, y, true, true) + 1);
        wall[x+1][y-1] = true;
    }
    return res;
}

int main()
{
    int T;
    scanf("%d", &T);
    for (int t = 1; t <= T; t++) {
        scanf("%d %d %d", &rows, &cols, &maxfall);
        for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            char c; scanf("\t%c", &c);
            wall[i][j] = c == '#';
        }

        int res = dig(0, 0, true, true);
        printf("Case #%d: ", t);
        if (res == INF)
            printf("No\n");
        else
            printf("Yes %d\n", res);
    }
    return 0;
}
