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

int a[55];
int b[55][55];
int n, p;
int c[55];
int ra[55][55], rb[55][55];

bool has_more() {
    Rep(i, n) if (c[i] >= p) return false;
    return true;
}

int main() {
    int nt;
    cin >> nt;
    Rep(t, nt) {
        cin >> n >> p;
        Rep(i, n) cin >> a[i];
        Rep(i, n) {
            Rep(j, p) cin >> b[i][j];
            sort(b[i], b[i] + p);
            Rep(j, p) {
                rb[i][j] = 10 * b[i][j] / (9 * a[i]);
                if ((10 * b[i][j]) % (11 * a[i]) == 0) {
                    ra[i][j] = (10 * b[i][j]) / (11 * a[i]);
                } else {
                    ra[i][j] = (10 * b[i][j]) / (11 * a[i]) + 1;
                }
            }
        }
        memset(c, 0, sizeof(c));

        int res = 0;

        while(has_more()) {
            int smallestB = 0;
            Rep(i, n) if (rb[i][c[i]] < rb[smallestB][c[smallestB]]) {
                smallestB = i;
            }
            bool ok = true;
            Rep(i, n) if (ra[i][c[i]] > rb[smallestB][c[smallestB]]) {
                ok = false;
            }
            if (ok) {
                Rep(i, n) ++c[i];
                ++res;
            } else {
                ++c[smallestB];
            }
        }

        cout << "Case #" << (t + 1) << ": " << res << endl;
    }
    return 0;
}
