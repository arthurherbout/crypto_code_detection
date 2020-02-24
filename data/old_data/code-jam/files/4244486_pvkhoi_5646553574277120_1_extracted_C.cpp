#include <bits/stdc++.h>
using namespace std;

int nTest;
int c, d, v;
int a[110];
map<int, int> f;

int main() {

    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);

    cin >> nTest;
    for (int test = 1; test <= nTest; test++) {
        cin >> c >> d >> v;
        for (int i = 1; i <= d; i++)
            cin >> a[i];
        sort(a + 1, a + 1 + d);

        long long possible = 0;
        int ret = 0;
        for (int i = 1; possible < v; ) {
            if (i <= d) {
                while (possible + 1 < a[i]) {
                    possible += c * (possible + 1LL);
                    ret++;
                }
                for (int j = 1; j <= c && possible < v; j++)
                    possible += a[i];
                i++;
            } else {
                while (possible < v) {
                    possible += c * (possible + 1LL);
                    ret++;
                }
            }
        }

        cout << "Case #" << test << ": " << ret << endl;
    }

    return 0;
}
