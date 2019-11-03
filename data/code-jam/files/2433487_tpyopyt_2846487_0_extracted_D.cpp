#include<cstdio>
#include<cstring>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<string>
#include<map>
#include<set>
using namespace std;
string s;
double f[5000000];
bool g[5000000];
int n;
int cf[100];

void dfs(int s)
{
     g[s] = false;
     if (s == 0) { f[s] = 0; return; }
     
     int i, v = 0, j;
     for (i = 0; i < n; i++)
         if (s & cf[i]) v++;
     for (i = 0; i < n; i++)
         for (j = 0; j < n; j++)
             if (s & cf[(i - j + n) % n]){
                int u = s - cf[(i - j + n) % n];
                if (g[u]) dfs(u);
                f[s] += 1 / double(n) * (n - j + f[u]);
                break;
             }
}

int main()
{
    freopen("D-small-attempt0.in", "r", stdin);
    freopen("D.out", "w", stdout);
    
    int ts, ks, i;
    cf[0] = 1;
    for (i = 1; i < 22; i++)cf[i] = cf[i - 1] * 2;
    cin >> ts;
    for (ks = 0; ks < ts; ks++){
        cin >> s;
        int zt = 0;
        n = s.length();
        for (i = 0; i < s.length(); i++)
            if (s[i] == 'X')
               zt = zt << 1;
            else zt = (zt << 1) + 1;
        for (i = 0; i < cf[n] ;i++){
            f[i] = 0;
            g[i] = true;
        }
        dfs(zt);
        cout << "Case #" << ks + 1 << ": ";
        printf("%.10lf", f[zt]);
        cout << endl;
    }
    return 0;
}
