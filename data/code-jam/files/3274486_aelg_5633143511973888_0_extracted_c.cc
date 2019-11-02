
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <set>
#include <utility>
#include <functional>
#include <algorithm>
#include <iomanip>

#define rep(i, a, b) for(int (i) = (a); (i) < (b); ++(i))

using namespace std;

void solve() {
    int n, k;
    int a, b;
    char dummy;
    cin >> n >> k;
    cin >> a >> dummy >> b;
    int u = a * 10000 + b;
    vector<int> ps;
    rep(i, 0, n){
        cin >> a >> dummy >> b;
        ps.push_back(a * 10000 + b);
    }
    while(u) {
        sort(ps.begin(), ps.end());
        auto m = min_element(ps.begin(), ps.end());
        for(auto &p : ps){
            if(*m == p) {
                ++p;
                --u;
                if(u == 0) break;
            }
        }
    }
    long double ans = 1;
    for(auto &p: ps){
        ans = ans * (static_cast<long double>(p)/10000);
    }
    cout << std::setprecision(std::numeric_limits<long double>::digits10 + 1) << ans << endl;
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        cout << "Case #" << i + 1 << ": ";
        solve();
    }
    return 0;
}
