#include <iostream>
#include <string>

std::string solve(std::string s) {

    int n = s.length();
    if (n == 0) {
        return s;
    }

    char largest = 0;
    int largest_index = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] > largest) {
            largest = s[i];
            largest_index = i;
        }
    }

    std::string ans;

    ans += solve(s.substr(0, largest_index));
    for (int i = largest_index; i < n; i++) {
        if (s[i] == largest) {
            ans = s[i] + ans;
        } else {
            ans += s[i];
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
        std::cout << "Case #" << t << ": " << solve(s) << std::endl;
    }

    return 0;
}

