#include <algorithm>
#include <iostream>
#include <vector>

const int max_height = 2500;

std::vector<int> solve(std::vector<std::vector<int>> lists) {

    int m = lists.size();
    int n = lists[0].size();

    std::vector<int> appears(max_height+1);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            appears[lists[i][j]]++;
        }
    }

    std::vector<int> ans;
    for (int i = 1; i <= max_height; i++) {
        if (appears[i]%2 == 1) {
            ans.push_back(i);
        }
    }
    std::sort(ans.begin(), ans.end());

    return ans;
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int N;
        std::cin >> N;
        std::vector<std::vector<int>> lists(2*N-1, std::vector<int>(N));
        for (int i = 0; i < 2*N-1; i++) {
            for (int j = 0; j < N; j++) {
                std::cin >> lists[i][j];
            }
        }
        std::vector<int> ans = solve(lists);
        std::cout << "Case #" << t << ":";
        for (int i = 0; i < N; i++) {
            std::cout << " " << ans[i];
        }
        std::cout << std::endl;
    }

    return 0;
}

