#include <cmath>
#include <iostream>
#include <algorithm>
using namespace std;

int ts, o;
int a[10][10], b[10][10], c[10], chs[10], lb[10], ta, tb, i, j, k, x, y, ans;

void out(int a[], int len) {
     for (int i=0; i<len; i++) cout << a[i] << ' ';
     cout << endl;
}

void chck() {
     int i, j;
     for (i=0; i<tb; i++)
         for (j=i+1; j<tb; j++)
             if (b[i][j] && !a[c[i]][c[j]] || a[c[i]][c[j]] && !b[i][j]) return;
//     out(c, ta);
//     cout << b[0][2] << ' ' << a[c[0]][c[2]] << endl;
     ans = 1;
}

void lab(int dp) {
     if (dp == ta) chck();
     if (ans) return;
     if (!chs[dp]) { lab(dp+1); return; }
     for (int i=0; i<tb; i++)
         if (!lb[i]) {
            lb[i] = 1;
            c[i] = dp;
            lab(dp+1);
            c[i] = -1;
            lb[i] = 0;
         }
}

main() {
       freopen("2.in", "r", stdin);
       freopen("2.out", "w", stdout);
       
       for (cin >> ts; ++o <= ts; ) {
           cout << "Case #" << o << ": ";
           memset(a, 0, sizeof(a));
           memset(b, 0, sizeof(b));
           cin >> ta;
           for (i=1; i<ta; i++) {
               cin >> x >> y;
               x--; y--;
               a[x][y] = a[y][x] = 1;
           }
           cin >> tb;
           for (i=1; i<tb; i++) {
               cin >> x >> y;
               x--; y--;
               b[x][y] = b[y][x] = 1;
           }
           ans = 0;
           for (i=0; i<(1 << ta); i++) {
               k = 0;
               for (j=0; j<ta; j++) {
                   chs[j] = (i & (1 << j)) > 0;
                   k += chs[j];
               }
//               cout << i << endl;
//               out(chs, ta);
               if (k != tb) continue;
               memset(lb, 0, sizeof(lb));
               memset(c, 0xff, sizeof(c));
               lab(0);
               if (ans) break;
           }
           if (ans) cout << "YES" << endl; else cout << "NO" << endl;
       }
}
