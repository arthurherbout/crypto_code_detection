#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stdio.h>

using namespace std;

long long f(long long r, long long h) {
    //long double y = 1.0;
    //cout << ((y * r) * r + (y * 2) * r * h) << endl;
    return (2 * r * h);
}

int main() {
    ifstream cin("input.in");
    ofstream cout("output.txt");
    cout.precision(30);
    int n1, n, k;
    long double have;
    cin >> n1;
    for (int t1 = 0; t1 < n1; t1++) {
        cin >> n >> k >> have;
        vector<long double> v(n);
        for (int i = 0; i < n; i++) {
            cin >> v[i];
        }
        long double left = 0;
        long double right = 1, mid;
        while (right - left > 0.0000001) {
                //cout << left << endl;
            mid = (right + left) / 2;
            long double r = 0;
            for (int j = 0; j < n; j++) {
                if (v[j] < mid) r += mid - v[j];
            }
            if (r < have) left = mid;
            else right = mid;
        }
        //cout << left << endl;
        long double r = 1;
        for (int j = 0; j < n; j++) {
            if (v[j] < right) r *= right;
            else r *= v[j];
            cout <<v[j] << " " << r << endl;
        }
        printf("Case #%d: %LLf\n", t1 + 1, r);
        //cout << "Case #" << t1 + 1 << ": " << r << endl;
    }

    return 0;
}
