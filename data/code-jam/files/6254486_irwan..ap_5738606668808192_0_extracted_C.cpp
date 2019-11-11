#include <iostream>
#include <string>
#include <vector>

// Generate polynomial from (x+1) * (x^14+...+1)

bool get_next(std::string& pattern) {
    int n = pattern.length();
    if (pattern[n-1] == '0') {
        pattern[n-1] = '1';
    } else {
        pattern[n-1] = '0';
        pattern[n-2] = '1';
    }
    for (int i = n-1; i >= 1; i--) {
        if (pattern[i] == '1' && pattern[i-1] == '1') {
            pattern[i] = '0';
            pattern[i-1] = '0';
            if (i == 1) {
                return false;
            }
            pattern[i-2] = '1';
        }
    }
    return true;
}

std::string get_number(const std::string& pattern) {
    std::string a = std::string("010") + pattern + std::string("01");
    std::string b = std::string("10") + pattern + std::string("010");
    int n = a.length();
    std::string ans(n, '0');
    for (int i = 0; i < n; i++) {
        if (a[i] == '1' || b[i] == '1') {
            ans[i] = '1';
        }
    }
    return ans;
}

int main() {

    int T;
    std::cin >> T; // ignore

    int N, J;
    std::cin >> N >> J;

    std::cout << "Case #1:" << std::endl;

    std::string pattern(N-5, '0');
    int printed = 0;
    while (printed < J) {
        std::string the_number = get_number(pattern);
        std::cout << the_number << " 3 4 5 6 7 8 9 10 11" << std::endl;
        printed++;
        get_next(pattern);
    }

    return 0;
}

