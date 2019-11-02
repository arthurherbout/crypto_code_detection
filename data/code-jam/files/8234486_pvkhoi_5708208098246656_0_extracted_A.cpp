#include <bits/stdc++.h>

using namespace std;

const int MAXN = 105;

bool b[MAXN][MAXN][4];
char a[MAXN][MAXN];
int r, c;

int get_dir(char c) {
   if (c == '^') return 0;
   if (c == '>') return 1;
   if (c == 'v') return 2;
   return 3;
}

void solve() {
   memset(b, true, sizeof b);
   for (int i = 1; i <= r; i++) {
      for (int j = 1; j <= c; j++) {
         if (a[i][j] == '.') continue;
         b[i][j][3] = false;
         break;
      }
      for (int j = c; j >= 1; j--) {
         if (a[i][j] == '.') continue;
         b[i][j][1] = false;
         break;
      }
   }
   for (int j = 1; j <= c; j++) {
      for (int i = 1; i <= r; i++) {
         if (a[i][j] == '.') continue;
         b[i][j][0] = false;
         break;
      }
      for (int i = r; i >= 1; i--) {
         if (a[i][j] == '.') continue;
         b[i][j][2] = false;
         break;
      }
   }
   int res = 0;
   for (int i = 1; i <= r; i++)
   for (int j = 1; j <= c; j++) {
      if (a[i][j] == '.') continue;
      if (!b[i][j][0] && !b[i][j][1] && !b[i][j][2] && !b[i][j][3]) {
         puts("IMPOSSIBLE"); return;
      }
      if (!b[i][j][get_dir(a[i][j])]) res++;
   }
   printf("%d\n", res);
}

int main() {
   freopen("input.txt", "r", stdin);
   freopen("output.txt", "w", stdout);

   int n_test; cin >> n_test;
   for (int t = 1; t <= n_test; t++) {
      scanf("%d%d\n", &r, &c);
      for (int i = 1; i <= r; i++)
         gets(a[i]+1);
      printf("Case #%d: ", t);
      solve();
   }

   return 0;
}
