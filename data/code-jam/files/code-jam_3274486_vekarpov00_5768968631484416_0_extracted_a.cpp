#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

long long f(long long r, long long h) {
    //long double y = 1.0;
    //cout << ((y * r) * r + (y * 2) * r * h) << endl;
    return (2 * r * h);
}

int main() {
    //ifstream cin("input.in");
    //ofstream cout("output.txt");
    cout.precision(50);
    int n1, n, k;
    cin >> n1;
    for (int t1 = 0; t1 < n1; t1++) {
        cin >> n >> k;
        vector<vector<long long> > v(n, vector<long long> (2));
        for (int i = 0; i < n; i++) {
            cin >> v[i][0] >> v[i][1];
        }
        //cout << k << endl;
        sort(v.rbegin(), v.rend());
        vector<vector<long long> > v1(n, vector<long long> (n + 1, 0));
        v1[0][1] = f(v[0][0], v[0][1]) + v[0][0] * v[0][0];
        //cout << v[0][0] << " " << v[0][1] << endl;
        //cout << "0 1 " << v1[0][1] << endl;
        //cout << 1 << endl;
        //cout << v1[0][1] << endl;
        for (int i = 1; i < n; i++) {
            v1[i][1] = max(f(v[i][0], v[i][1]) + v[i][0] * v[i][0], v1[i - 1][1]);
            //cout << v1[i][1] << endl;
        }
        for (int i = 1; i < n; i++) {
            for (int j = 2; j <= n; j++) {
                v1[i][j] = max(v1[i - 1][j], v1[i - 1][j - 1] + f(v[i][0], v[i][1]));
                //cout << i << " " << j << " " << v1[i][j] << endl;
            }
        }
        //cout << k << endl;
        //cout << 2 << endl;

        long double pi = 3.1415926535897932384626433832795028841971;
        pi *= v1[n - 1][k];
        //cout << 3.1415926 * v1[n - 1][k] << endl;
        cout << "Case #" << t1 + 1 << ": " << pi << endl;
    }

    return 0;
}
