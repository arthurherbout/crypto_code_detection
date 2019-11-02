#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstring>
#include <algorithm>

using namespace std;

void solve(const int e) {
    cout << "Case #" << e << ": ";

    int n, k; cin >> n >> k;
    double u; cin >> u;
    double s[55];
    for (int i=0; i<n; ++i)
        cin >> s[i];

    sort(s, s+n);

    double mn = s[0];
    double mx = 1.0;

    for (int q=0; q<88; ++q) {
        double md = (mn + mx) * 0.5;

        double left = u;
        for (int i=0; i<n; ++i) {
            left -= max(0.0, md - s[i]);
        }

        if (left > 0)
            mn = md;
        else
            mx = md;
    }

    long double p = 1.0;
    for (int i=0; i<n; ++i) {
        //cerr << max(mn, s[i]) << ' ';
        p *= max(mn, s[i]);
    }
    //cerr << endl;

    cout << fixed << setprecision(9) << p << endl;
}

int main() {
    int t; cin >> t;
    for (int e=1; e<=t; ++e)
        solve(e);
}

