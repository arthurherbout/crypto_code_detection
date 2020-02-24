#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <set>
#include <vector>
#include <cstring>
#include <string>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <map>
#include <queue>
using namespace std;
typedef long long ll;
typedef vector<int> vi;
typedef vector<ll> vl;
typedef vector<vi> vvi;
typedef vector<vl> vvl;
typedef vector<double> vd;
typedef pair<int, int> pii;
typedef pair<double, double> pdd;
typedef vector<pii> vii;
typedef vector<string> vs;

int main() {
    int add, T;
    cin >> add >> T;
    for (int test = 1; test <= T; ++test) {
        printf("Case #%d: ", test + add);
        int y,n;
        cin >> y >> n;
        vi p(n),s(n);
        vi c0(2);
        vvi rem0(2);
        vvi ind(2);
        for (int i = 0; i < n; ++i) {
            cin >> p[i];
            if (p[i] < 0) {
                ++c0[0];
                ind[0].push_back(i);
                rem0[0].push_back(1);
            } else {
                ++c0[1];
                ind[1].push_back(i);
                rem0[1].push_back(1);
            }
        }
        for (int i = 0; i < n; ++i) cin >> s[i];
        double res = 1e50;
        for (int mask = 0; mask < (1 << n); ++mask) {
            if (__builtin_popcount(mask) != c0[1]) continue;
            double t = 0, x = 0;
            vvi rem = rem0;
            vi c = c0;
            for (int i = 0; i < n; ++i) {
                int type = 0;
                if (c[0] + c[1] == 0) break;
                if (((mask & (1 << i)) && c[1]) || c[0] == 0) {
                    type = 1;
                }
                if (type) {
                    int best = 0;
                    double mi = 1e50;
                    for (int i = 0; i < ind[1].size(); ++i) if (rem[1][i]) {
                        int j = ind[1][i];
                        double tt = (p[j] + t * s[j] - x) / (y - s[j]);
                        if (tt < mi) {
                            mi = tt;
                            best = i;
                        }
                    }
                    rem[1][best] = 0;
                    t += mi;
                    x += mi*y;
                    --c[1];
                } else {
                    int best = 0;
                    double mi = 1e50;
                    for (int i = 0; i < ind[0].size(); ++i) if (rem[0][i]) {
                        int j = ind[0][i];
                        double tt = abs((p[j] - t * s[j] - x) / (y - s[j]));
                        if (tt < mi) {
                            mi = tt;
                            best = i;
                        }
                    }
                    rem[0][best] = 0;
                    t += mi;
                    x -= mi*y;
                    --c[0];
                }
            }
            assert(c[0] == 0 && c[1] == 0);
//            cerr << c[0] << ' ' << c[1] << endl; 
            res = min(res, t);
        }
        printf("%.8lf\n", res);
    }
    return 0;
}
