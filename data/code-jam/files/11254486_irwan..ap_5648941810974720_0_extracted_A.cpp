#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>


std::string solve(std::string s) {

    static std::string key = "ZXWUFVROGI";
    static std::string values = "0624573189";
    static std::vector<std::string> words = {
        "ZERO",
        "SIX",
        "TWO",
        "FOUR",
        "FIVE",
        "SEVEN",
        "THREE",
        "ONE",
        "EIGHT",
        "NINE"
    };

    std::string ans;
    std::vector<int> freq(26);
    int len = s.length();
    for (int i = 0; i < len; i++) {
        freq[s[i]-'A']++;
    }

    for (int i = 0; i < 10; i++) {
        int index = key[i]-'A';
        while (freq[index] > 0) {
            ans += values[i];
            int len = words[i].length();
            for (int j = 0; j < len; j++) {
                freq[words[i][j]-'A']--;
            }
        }
    }

    std::sort(ans.begin(), ans.end());
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

