#include <iostream>
#include <cstring>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

int read() {
    string s; cin >> s;
    s.erase(remove(s.begin(), s.end(), '.'), s.end());
    return stoi(s);
}

struct Flow {
    long long speed;
    long long temperature;

    bool operator < (const Flow& ot) const {
        return temperature < ot.temperature;
    }
};

Flow s[101];

void solve(int e) {
    int n; cin >> n;
    int V = read();
    int T = read();

    cerr << "Test " << e << endl;
    cerr << "V and X = " << V << ' ' << T << endl;

    for (int i=0; i<n; ++i) {
        s[i].speed = read();
        s[i].temperature = read();
        cerr << s[i].speed << ' ' << s[i].temperature << endl;
    }

    sort(s, s+n);

    cout << "Case #" << e << ": ";

    if (n == 1) {
        if (T == s[0].temperature) {
            cout << fixed << setprecision(8) << double(V) / s[0].speed << endl;
        } else {
            cout << "IMPOSSIBLE" << endl;
        }
    } else {
        if (s[n-1].temperature < T) {
            cout << "IMPOSSIBLE" << endl;
        } else if (s[0].temperature > T) {
            cout << "IMPOSSIBLE" << endl;
        } else {
            if (s[0].temperature == T || s[1].temperature == T) {
                double sp = 0;
                if (s[0].temperature == T)
                    sp += s[0].speed;
                if (s[1].temperature == T)
                    sp += s[1].speed;
                cout << fixed << setprecision(8) << double(V) / sp << endl;
            } else {
                double A = double(T - s[n-1].temperature) / (s[0].temperature - T);

                double V2 = double(V) / (A + 1);
                double V1 = A * V2;

                cerr << "V1 and V2 = " << V1 << ' ' << V2 << endl;

                double t1 = V1 / s[0].speed;
                double t2 = V2 / s[n-1].speed;

                cout << fixed << setprecision(8) << max(t1, t2) << endl;
            }
        }
    }
}

int main() {
    int t; cin >> t;
    for (int e=1; e<=t; ++e)
        solve(e);

    return 0;
}



