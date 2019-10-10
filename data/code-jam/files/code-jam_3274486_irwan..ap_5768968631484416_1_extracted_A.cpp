#include <algorithm>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <vector>

struct pancake {
    int r;
    int h;
    bool operator<(const pancake& other) const {
        return ((long long int)r * (long long int)h)
                > ((long long int)other.r * (long long int)other.h);
    }
};

double solve(std::vector<pancake> pancakes, int k) {
    double ans = 0.0;
    int n = pancakes.size();
    for (int base = 0; base < n; base++) {
        std::vector<pancake> others;
        for (int i = 0; i < n; i++) {
            if (i == base) {
                continue;
            }
            if (pancakes[i].r > pancakes[base].r) {
                continue;
            }
            others.push_back(pancakes[i]);
        }
        if (others.size() < k-1) {
            continue;
        }
        std::sort(others.begin(), others.end());
        double area = 0.0;
        area += M_PI * pancakes[base].r * pancakes[base].r;
        area += 2 * M_PI * pancakes[base].r * pancakes[base].h;
        for (int i = 0; i < k-1; i++) {
            area += 2 * M_PI * others[i].r * others[i].h;
        }
        if (area > ans) {
            ans = area;
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
        std::vector<pancake> pancakes(N);
        for (int i = 0; i < N; i++) {
            std::cin >> pancakes[i].r >> pancakes[i].h;
        }
        double ans = solve(pancakes, K);
        printf("Case #%d: %.9lf\n", t, ans);
    }

    return 0;
}

