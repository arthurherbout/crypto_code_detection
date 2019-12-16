#include <algorithm>
#include <iostream>
#include <vector>

bool check(const std::vector<int>& participants, const std::vector<int>& bff) {
    int n = participants.size();
    if (n <= 1) {
        return true;
    }
    for (int i = 0; i < n; i++) {
        int prev = i-1;
        if (prev == -1) {
            prev = n-1;
        }
        int next = i+1;
        if (next == n) {
            next = 0;
        }
        if (participants[prev] != bff[participants[i]] && participants[next] != bff[participants[i]]) {
            return false;
        }
    }
    return true;
}

bool check_all(std::vector<int> participants, const std::vector<int>& bff) {

    do {
        bool ok = check(participants, bff);
        if (ok) {
            return true;
        }
    } while (std::next_permutation(participants.begin(), participants.end()));
    return false;
}

int solve(std::vector<int> bff) {

    int n = bff.size();
    int ans = 0;

    for (int bp = 0; bp < (1<<n); bp++) {
        std::vector<int> participants;
        for (int i = 0; i < n; i++) {
            if (bp & (1<<i)) {
                participants.push_back(i);
            }
        }
        bool ok = check_all(participants, bff);
        if (ok) {
            int m = participants.size();
            if (m > ans) {
                ans = participants.size();
            }
        }
    }

    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int N;
        std::cin >> N;
        std::vector<int> bff(N);
        for (int i = 0; i < N; i++) {
            std::cin >> bff[i];
            bff[i]--;
        }
        int ans = solve(bff);
        std::cout << "Case #" << t << ": " << ans << std::endl;
    }

    return 0;
}

