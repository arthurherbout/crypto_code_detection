# include <iostream>
# include <cmath>
# include <cstring>
# include <string>
# include <vector>
# include <set>
# include <queue>
# include <stack>
# include <algorithm>
# include <list>
# include <map>
# include <sstream>
# define N 50
# define M 1<<6
using namespace std;

char c[N][N+1];
int v[N][N][M][M], n, m, lim;

int recursion(int x, int y, int caida)
{
    int i, j, k;
    if (caida>lim) {
        return 1000000;
    }
    if (x==n-1 && y==m-1) {
        return 0;
    }
    k=0;
    for (i=0; i<m; i++) {
        k=k*2+(c[x][i]=='#');
    }
    j=0;
    if (x+1<n) {
        for (i=0; i<m; i++) {
            j=j*2+(c[x+1][i]=='#');
        }
    }
    int& best=v[x][y][j][k];
    if (best!=-1) {
        return best;
    }
    best=1000000;
    if (x+1<n && c[x+1][y]=='.') {
        best=min(best, recursion(x+1, y, caida+1));
        return best;
    }
    if (x+1<n && y+1<m && c[x+1][y+1]=='#' && c[x][y+1]!='#') {
        c[x+1][y+1]='.';
        best=min(best, 1+recursion(x, y, 0));
        c[x+1][y+1]='#';
    }
    if (x+1<n && y-1>=0 && c[x+1][y-1]=='#' && c[x][y-1]!='#') {
        c[x+1][y-1]='.';
        best=min(best, 1+recursion(x, y, 0));
        c[x+1][y-1]='#';
    }
    if (y+1<m && c[x][y+1]!='#') {
        best=min(best, recursion(x, y+1, 0));
    }
    if (y-1>=0 && c[x][y-1]!='#') {
        best=min(best, recursion(x, y-1, 0));
    }
    return best;
}
    
int main()
{
    int t, tt=1, i, j;
    scanf("%d", &t);
    while(t--) {
        scanf("%d%d%d", &n, &m, &lim);
        for (i=0; i<n; i++) {
            scanf("%s", c[i]);
        }
        memset(v, -1, sizeof v);
        int pp=recursion(0, 0, 0);
        if (pp<1000000) {
            printf("Case #%d: Yes %d\n", tt++, pp);
        }
        else {
            printf("Case #%d: No\n", tt++);
        }
    }
    return 0;
}
