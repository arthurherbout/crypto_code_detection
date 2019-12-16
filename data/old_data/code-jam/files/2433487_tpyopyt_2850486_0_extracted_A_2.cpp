#include<cstdio>
#include<cstring>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<string>
#include<map>
#include<set>
using namespace std;
#define ll long long
ll a[100], b, now, n, i, j, c[100];
double ans, xans;

void make(ll lim)
{
     ll i;
     for (i = 0; i < n; i++) c[i] = a[i];
     for (i = n; i < 37; i++) c[i] = 0;
     sort(c, c + 37);
     i = 0;
     ll money = b;
     while (i < 37 && c[i] < lim){
           money -= lim - c[i];
           i++;
     }
     if (money < 0) return;
     ll v = 0, u = 0;
     for (i = 0; i < 37; i++){
         if (c[i] <= lim) v++;
         if (c[i] == lim) u++;
     }
     for (j = 0; j <= v; j++){
         if (money < j || v == j) break;
         xans = 0;
         for (i = 0; i < 37; i++)
             if (c[i] < lim && i < v - j){
                xans += (double)(lim - c[i]) * 36 / (v - j);
                xans -= lim - c[i];
                c[i] = lim;
             }
         xans -= j;
         if (xans > ans) ans = xans;
     }
}

int main()
{
    freopen("A-small-attempt10.in", "r", stdin);
    freopen("A.out", "w", stdout);
    ll ts, ks;
    cin >> ts;
    for (ks = 0; ks < ts; ks++){
        cin >> b >> n;
        for (i = 0; i < n; i++) cin >> a[i];
        sort(a, a + n);
        ans = 0;
        for (i = 0; i < n; i++){
            make(a[i]);
            if (a[i] > 0) make(a[i] - 1);
        }
         ll j, lef, rig, mid, money;
            lef = 0; rig = b + a[n - 1];
            while (lef < rig){
                  mid = (lef + rig + 1) / 2;
                  money = b;
                  for (j = 0; j < n; j++)
                      if (mid > a[j]) money -= mid - a[j];
                  for (j = n; j < 37; j++)
                      money -= mid;
                  if (money >= 0)
                     lef = mid;
                  else rig = mid - 1;
            }
            make(lef);
        cout << "Case #" << ks + 1 << ": ";
        printf("%.10lf", ans);
        cout << endl;
    }
    return 0;
}
