#include <iostream>
#include <sstream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <queue>
#include <algorithm>
#include <cstring>
#include <cmath>
using namespace std;

#define Rep(i,n) for(int i=0;i<(n);++i)
#define For(i,a,b) for(int i=(a);i<=(b);++i)
#define Ford(i,a,b) for(int i=(a);i>=(b);--i)
#define fi first
#define se second
#define pb push_back
#define MP make_pair

typedef pair<int,int> PII;
typedef vector<int> VI;
typedef long long LL;

const int MAX = 222;

LL hd, ad, hk, ak, b, d;

int main() {
    int nt;
    cin >> nt;
    Rep(t, nt) {
        cin >> hd >> ad >> hk >> ak >> b >> d;
        LL inf = 1000000000;
        LL best = inf;
        Rep(dc, MAX) Rep(bc, MAX) {
            LL chd = hd, cad = ad, chk = hk, cak = ak;
            LL cure = 0;
            bool fail = false;
            Rep(i, dc) {
                LL newcak = max(cak - d, 0LL);
                if (chd > newcak) {
                    cak = newcak;
                    chd = chd - newcak;
                } else if (hd - cak > newcak) {
                    cure++;
                    chd = hd - cak - newcak;
                    cak = newcak;
                } else {
                    fail = true;
                }
            }
            Rep(i, bc) {
                LL newcad = cad + b;
                if (chd > cak) {
                    cad = newcad;
                    chd = chd - cak;
                } else if (hd - cak > cak) {
                    cure++;
                    chd = hd - cak - cak;
                    cad = newcad;
                } else {
                    fail = true;
                }
            }

            if (!fail) {
                Rep(i, MAX) {
                    if (chk <= cad && !fail) {
                        best = min(best, cure + bc + dc + i + 1);
                    } else {
                        if (chd > cak) {
                            chd -= cak;
                            chk -= cad;
                        } else if (hd - cak > cak) {
                            cure++;
                            chd = hd - cak - cak;
                            chk -= cad;
                        } else {
                            fail = true;
                        }
                    }
                }
            }
        }
        cout << "Case #" << (t + 1) << ": ";
        if (best == inf) {
            cout << "IMPOSSIBLE" << endl;
        } else {
            cout << best << endl;
        }
    }
    return 0;
}
