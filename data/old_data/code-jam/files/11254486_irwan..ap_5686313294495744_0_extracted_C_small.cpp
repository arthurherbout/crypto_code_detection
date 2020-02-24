#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <utility>

bool valid(const std::vector<std::pair<std::string,std::string>>& topics, int fake) {

    int n = topics.size();
    std::set<std::string> first;
    std::set<std::string> second;

    for (int i = 0; i < n; i++) {
        if (((1<<i) & fake) == 0) {
            // original
            first.insert(topics[i].first);
            second.insert(topics[i].second);
        }
    }

    for (int i = 0; i < n; i++) {
        if (((1<<i) & fake) > 0) {
            // fake
            if (first.find(topics[i].first) == first.end()) {
                return false;
            }
            if (second.find(topics[i].second) == second.end()) {
                return false;
            }
        }
    }

    return true;
}

int bitcount(int bit, int n) {
    int ans = 0;
    for (int i = 0; i < n; i++) {
        if (((1<<i) & bit) > 0) {
            ans++;
        }
    }
    return ans;
}

int solve(std::vector<std::pair<std::string,std::string>> topics) {

    int ans = 0;

    int n = topics.size();
    for (int i = 0; i < (1<<n); i++) {
        if (valid(topics, i)) {
            int bc = bitcount(i, n);
            if (bc > ans) {
                ans = bc;
            }
        }
    }

    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int n;
        std::cin >> n;
        std::vector<std::pair<std::string,std::string>> topics(n);
        for (int i = 0; i < n; i++) {
            std::cin >> topics[i].first >> topics[i].second;
        }
        int ans = solve(topics);
        std::cout << "Case #" << t << ": " << ans << std::endl;
    }

    return 0;
}

