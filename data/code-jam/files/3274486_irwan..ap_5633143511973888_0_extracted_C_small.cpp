#include <algorithm>
#include <cstdio>
#include <iostream>
#include <vector>

double solve(std::vector<double> p, double u) {

    std::sort(p.begin(), p.end());
    int n = p.size();
    
    double average = u/n;
    for (int i = 0; i < n; i++) {
        average += p[i]/n;
    }
    if (average >= 1.0) {
        return 1.0;
    }

    if (p[n-1] <= average) {
        double ans = 1.0;
        for (int i = 0; i < n; i++) {
            ans *= average;
        }
        return ans;
    }

    double total_p = u;
    int train_unit = 0;
    for (int i = 0; i < n; i++) {
        if (p[i] > average) {
            break;
        }
        total_p += p[i];
        train_unit++;
    }
    average = total_p / train_unit;
    double ans = 1.0;
    for (int i = 0; i < n; i++) {
        if (i < train_unit) {
            ans *= average;
        } else {
            ans *= p[i];
        }
    }
    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int N, K;
        std::cin >> N >> K;
        double U;
        std::cin >> U;
        std::vector<double> p(N);
        for (int i = 0; i < N; i++) {
            std::cin >> p[i];
        }
        double ans = solve(p, U);
        printf("Case #%d: %.9lf\n", t, ans);
    }

    return 0;
}

