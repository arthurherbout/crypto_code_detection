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
    int T;
    cin >> T;
    for (int test = 1; test <= T; ++test) {
        printf("Case #%d: ", test);
        int n,k;
        cin >> n >> k;
        vl s(n-k+1);
        for (int i = 0; i < s.size(); ++i) cin >> s[i];
        vl mi(k), ma(k);
        for (int b = 0; b < k; ++b) {
            ll add = 0;
            for (int i = b + 1; i < s.size(); i += k) {
                add += s[i] - s[i-1];
                mi[b] = min(mi[b], add);
                ma[b] = max(ma[b], add);
            }
        }
        //ll res = 0, r = -s[0], r1 = -s[0];
        ll res = 0, r = 0;
        for (int i = 0; i < k; ++i) {
            res = max(res, ma[i] - mi[i]);
            r -= mi[i];
//            r1 += ma[i];
//            cerr << mi[i] << ' ' << ma[i] << endl;
        }
        r = (r%k + k) % k;
        ll rs = (s[0]%k + k) % k;
//        r1 = (k - r1%k) % k;
        ll cnt = 0;
        for (int i = 0; i < k; ++i) cnt += (res - (ma[i] - mi[i]));
//        cerr << res << ' ' << r << ' ' << cnt << endl;
        if (cnt < k && r != rs && ((r < rs && r + cnt < rs) || (r > rs && r + cnt - k < rs))) {
            ++res;
//            assert(cnt < r1);
        } else {
//            assert(cnt >= r1);
            vl x(n);
            ll sum = 0;
            for (int i = 1; i < k; ++i) {
                x[i] = x[0] + mi[0] - mi[i];
                sum += x[i];
            }
            for (int i = 0; i < k && (s[0] - sum) % k; ++i) {
                int c = res - (ma[i] - mi[i]);
                while (c && (s[0] - sum) % k) {
                    --c;
                    ++x[i];
                    ++sum;
                }
            }
            assert((s[0] - sum) % k == 0);
            ll d = (s[0] - sum) / k;
            sum = 0;
            for (int i = 0; i < k; ++i) {
                x[i] += d;
                sum += x[i];
            }
            assert(sum == s[0]);
            for (int i = k; i < n; ++i) x[i] = x[i-k] + s[i-k+1] - s[i-k];
            ll m1 = x[0], m2 = x[0];
            for (int i = 0; i < n; ++i) {
                m1 = min(m1, x[i]);
                m2 = max(m2, x[i]);
//                cerr << x[i] << ' ';
            }
//            cerr << res << ' ' << m1 << ' ' << m2 << endl;
            assert(m2 - m1 == res);
        }
        cout << res << endl;
    }   
    return 0;
}
