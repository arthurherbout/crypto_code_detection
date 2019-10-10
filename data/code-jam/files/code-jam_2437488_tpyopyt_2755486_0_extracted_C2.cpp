#include<cstdio>
#include<cstring>
#include<iostream>
#include<cmath>
#include<algorithm>
#include<string>
#include<map>
#include<set>
using namespace std;
const int MAXN = 1500000 + 10;
#define ll long long
ll LLMAX;
set<ll> loc;
ll am[MAXN * 4], fg[MAXN * 4];
bool bo[MAXN * 4];
int n, gs;
struct node{ int t, s; ll w, e;} a[MAXN];
ll b[MAXN];

void reloc(ll &i)
{
     int lef, rig, mid;
     lef = 1; rig = gs;
     while (lef < rig)
     {
           mid = (lef + rig) / 2;
           if (i <= b[mid])
              rig = mid;
           else lef = mid + 1;
     }
     i = lef;
}

bool cmp(node a, node b)
{
     if (a.t < b.t) return true;
     return false;
}

ll scan(int x, int y)
{
     ll s = LLMAX;
     for (int i = x; i <= y; i++)
         if (am[i] < s) s = am[i];
     return s;
}

void ins(int x, int y, int z)
{
     for (int i = x; i <= y; i++)
         am[i] = z;
}

int main()
{
    freopen("C-small-attempt1.in", "r", stdin);
    freopen("C1.out", "w", stdout);
    LLMAX = 999999999 * 999999999;
    int ans;
    int ts, ks, i, j, k;
    int _d, _n, _w, _e, _s, _d1, _d2, _d3;
    cin >> ts;
    for (ks = 0; ks < ts; ks++){
        cin >> n;
        loc.clear();
        int num = 0;
        for (i = 0; i < n; i++){
            cin >> _d >> _n >> _w >> _e >>_s >> _d1 >> _d2 >> _d3;
            for (j = 0; j < _n; j++){
                a[num].t = _d + j * _d1;
                a[num].e = _e + j * _d2 - 1;
                a[num].w = _w + j * _d2;
                a[num].s = _s + j * _d3;
                if (loc.find(a[num].e) == loc.end())
                   loc.insert(a[num].e);
                if (loc.find(a[num].w) == loc.end())
                   loc.insert(a[num].w);
                num++;
            }
        }
        
        gs = 0;
        for (set<ll>::iterator iter = loc.begin(); iter != loc.end(); iter++)
            b[++gs] = *iter;
        for (i = 0; i < num; i++){
            reloc(a[i].w);
            reloc(a[i].e);
        }
        
        sort(a, a + num, cmp);
        i = 0;
        ans = 0;
        memset(am, 0, sizeof(am));
        while (i < num){
              j = i;
              while (j < num && a[j].t == a[i].t){
                    if (scan(a[j].w, a[j].e) < a[j].s){
                       ans++;
                  //     cout << "AA: " << ans << " " << j << endl;
                       }
                    j++;
              }
              for (k = i; k < j; k++)
                  if (scan(a[k].w, a[k].e) < a[k].s)
                     ins(a[k].w, a[k].e, a[k].s);
              i = j;
        }
        cout << "Case #" << ks + 1 << ": " << ans << endl;
    }
    return 0;
}
