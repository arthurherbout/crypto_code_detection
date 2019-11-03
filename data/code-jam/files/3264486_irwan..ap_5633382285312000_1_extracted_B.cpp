#include <iostream>

long long int solve(long long int x) {
    int d[21];
    for (int i = 0; i <= 20; i++) {
        d[i] = x % 10;
        x /= 10;
    }
    for (int i = 20; i >= 1; i--) {
        if (d[i-1] < d[i]) {
            d[i]--;
            for (int j = i-1; j >= 0; j--) {
                d[j] = 9;
            }
            int j = i;
            while (d[j+1] > d[j]) {
                d[j+1]--;
                d[j] = 9;
                j++;
            }
            break;
        }
    }
    long long int ans = 0;
    for (int i = 20; i >= 0; i--) {
        ans = ans*10 + d[i];
    }
    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        long long int n;
        std::cin >> n;
        std::cout << "Case #" << t << ": " << solve(n) << std::endl;
    }

    return 0;
}

