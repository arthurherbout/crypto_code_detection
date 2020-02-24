#include <iostream>
#include <string>

int solve(std::string s, int n) {
    int ans = 0;
    int len = s.length();

    for (int i = 0; i+n-1 < len; i++) {
        if (s[i] == '-') {
            ans++;
            for (int j = i; j < i+n; j++) {
                s[j] = (s[j] == '+') ? '-' : '+';
            }
        }
    }

    for (int i = 0; i < len; i++) {
        if (s[i] == '-') {
            return -1;
        }
    }
    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        std::string s;
        int n;
        std::cin >> s >> n;
        int ans = solve(s, n);
        std::cout << "Case #" << t << ": ";
        if (ans < 0) {
            std::cout << "IMPOSSIBLE";
        } else {
            std::cout << ans;
        }
        std::cout << std::endl;
    }

    return 0;
}

