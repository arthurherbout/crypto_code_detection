#include <stdio.h>
#include <algorithm>
#include <set>

using namespace std;

int n, m;
int ans;

char map[50][50];
int flag[50][50], lp, rp;

int l[2500], r[2500];
int ind[2500], to[10000], next[10000], p;

int used[2500], op[2500][2];

int dfs (int x) {
//    printf("x = %d\n",x);
    used[x] = 1;
    for (int i = ind[x]; i != -1; i = next[i])
        if (op[to[i]][0] == -1 || op[to[i]][1] == -1 && r[to[i]] == 2 && op[to[i]][0] != x) {
            if (op[to[i]][0] == -1)
                op[to[i]][0] = x;
            else
                op[to[i]][1] = x;
            return 1;
            }
    for (int i = ind[x]; i != -1; i = next[i])
        if (op[to[i]][1] != x && !used[op[to[i]][0]] && dfs(op[to[i]][0])) {
            op[to[i]][0] = x;
            return 1;
            }
    for (int i = ind[x]; i != -1; i = next[i])
        if (r[to[i]] == 2 && op[to[i]][0] != x && op[to[i]][1] != -1 && !used[op[to[i]][1]] && dfs(op[to[i]][1])) {
            op[to[i]][1] = x;
            return 1;
            }
    return 0;
    }

int main () {
    int ct = 0, t;
    
    freopen("e.in", "r", stdin);
    freopen("e.out", "w", stdout);
    
    for (scanf("%d", &t); t > 0; t --) {
        
        scanf("%d%d",&n,&m);
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                do scanf("%c", map[i] + j);
                    while (map[i][j] <= ' ');

        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                if (map[i][j] == '?') {
                    int c = 4;
                    c -= (i - 1 >= 0 && map[i-1][j] == '#');
                    c -= (i + 1 < n && map[i+1][j] == '#');
                    c -= (j - 1 >= 0 && map[i][j-1] == '#');
                    c -= (j + 1 < m && map[i][j+1] == '#');
                    
                    if (c == 3)
                        map[i][j] = '1';
                    else
                        if (c == 4)
                            map[i][j] = '2';
                        else
                            map[i][j] = '.';
                    }
/*
        for (int i = 0; i < n; i ++) {
            for (int j = 0; j < m; j ++)
                printf("%c",map[i][j]);
            printf("\n");
            }
*/
        ans = 0;
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                if (map[i][j] == '#') {
                    ans += 4;
                    ans -= (i - 1 >= 0 && map[i-1][j] == '#');
                    ans -= (i + 1 < n && map[i+1][j] == '#');
                    ans -= (j - 1 >= 0 && map[i][j-1] == '#');
                    ans -= (j + 1 < m && map[i][j+1] == '#');
                    }
                else
                    if (map[i][j] == '1')
                        ans += 2;
                    else
                        if (map[i][j] == '2')
                            ans += 4;
        lp = rp = 0;
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                if (map[i][j] == '1' || map[i][j] == '2') {
                    if ((i+j)&1) {
                        l[lp] = map[i][j] - '0';
                        flag[i][j] = lp ++;
                        }
                    else {
                        r[rp] = map[i][j] - '0';
                        flag[i][j] = rp ++;
                        }
                    }
//        printf("lp = %d, rp = %d\n",lp,rp);
        for (int i = 0; i < lp; i ++)
            ind[i] = -1;
        for (int i = 0; i < n; i ++)
            for (int j = 0; j < m; j ++)
                if ((map[i][j] == '1' || map[i][j] == '2') && ((i+j)&1)) {
                    if (i - 1 >= 0 && (map[i-1][j] == '1' || map[i-1][j] == '2')) {
                        next[p] = ind[flag[i][j]];
                        to[p] = flag[i-1][j];
                        ind[flag[i][j]] = p ++;
                        }
                    if (i + 1 < n && (map[i+1][j] == '1' || map[i+1][j] == '2')) {
                        next[p] = ind[flag[i][j]];
                        to[p] = flag[i+1][j];
                        ind[flag[i][j]] = p ++;
                        }
                    if (j - 1 >= 0 && (map[i][j-1] == '1' || map[i][j-1] == '2')) {
                        next[p] = ind[flag[i][j]];
                        to[p] = flag[i][j-1];
                        ind[flag[i][j]] = p ++;
                        }
                    if (j + 1 < m && (map[i][j+1] == '1' || map[i][j+1] == '2')) {
                        next[p] = ind[flag[i][j]];
                        to[p] = flag[i][j+1];
                        ind[flag[i][j]] = p ++;
                        }
                    }
        
//        printf("p = %d\n", p);
        
        for (int i = 0; i < rp; i ++)
            op[i][0] = op[i][1] = -1;
        for (int i = 0; i < lp; i ++)
            for (int j = 0; j < l[i]; j ++) {
                for (int k = 0; k < lp; k ++)
                    used[k] = 0;
                ans -= dfs(i) * 2;
                }

//        for (int i = 0; i < rp; i ++)
//            printf("%d %d\n",op[i][0],op[i][1]);
        
        printf("Case #%d: %d\n", ++ct, ans);
        }
   
    return 0;
    }
/*
15 5
#....
.##..
#..##
.##..
#.#.#
##.#.
.##.#
#....
..#..
##..#
...##
.##.#
#....
##.#.
.##..
*/
