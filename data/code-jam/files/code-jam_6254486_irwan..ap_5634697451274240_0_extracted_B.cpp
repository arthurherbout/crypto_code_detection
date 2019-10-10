#include <iostream>
#include <string>

int solve(std::string s) {
    s += '+';
    int n = s.length();
    int ans = 0;
    for (int i = 1; i < n; i++) {
        if (s[i-1] != s[i]) {
            ans++;
        }
    }
    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        std::string s;
        std::cin >> s;
        int ans = solve(s);
        std::cout << "Case #" << t << ": " << ans << std::endl;
    }

    return 0;
}

