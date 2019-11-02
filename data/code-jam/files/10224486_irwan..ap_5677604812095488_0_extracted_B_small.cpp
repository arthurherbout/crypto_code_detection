#include <algorithm>
#include <iostream>
#include <vector>
#include <cstdio>

double find_probability(std::vector<double> p) {
    for (int i = 0; i < p.size(); i++) {
    }
    int n = p.size();
    double ans = 0.0;
    for (int i = 0; i < (1<<n); i++) {
        int yes_count = 0;
        double prob = 1.0;
        for (int j = 0; j < n; j++) {
            if (i & (1<<j)) {
                yes_count++;
                prob *= p[j];
            } else {
                prob *= (1-p[j]);
            }
        }
        if (2*yes_count == n) {
            ans += prob;
        }
    }
    return ans;
}

double solve(int N, int K, std::vector<double> p) {
    double ans = 0.0;
    for (int i = 0; i < (1<<N); i++) {
        std::vector<double> chosen;
        for (int j = 0; j < N; j++) {
            if (i & (1<<j)) {
                chosen.push_back(p[j]);
            }
        }
        if (chosen.size() == K) {
            double prob = find_probability(chosen);
            ans = std::max(ans, prob);
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
        std::vector<double> p(N);
        for (int i = 0; i < N; i++) {
            std::cin >> p[i];
        }
        double ans = solve(N, K, p);
        std::printf("Case #%d: %.9lf\n", t, ans);
    }

    return 0;
}

