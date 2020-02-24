#include <iostream>

int update_seen(int seen, int x) {
    while (x > 0) {
        int d = x%10;
        x /= 10;
        seen |= (1<<d);
    }
    return seen;
}

int solve(int n) {
    if (n == 0) {
        return -1;
    }
    int ans = 0;
    int seen = 0;
    do {
        ans += n;
        seen = update_seen(seen, ans);
    } while (seen != (1<<10)-1);
    return ans;
}

int main() {

    int T;
    std::cin >> T;
    for (int t = 1; t <= T; t++) {
        int n;
        std::cin >> n;
        int ans = solve(n);
        std::cout << "Case #" << t << ": ";
        if (ans == -1) {
            std::cout << "INSOMNIA";
        } else {
            std::cout << ans;
        }
        std::cout << std::endl;
    }

    return 0;
}

