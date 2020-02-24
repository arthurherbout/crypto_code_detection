#include <iostream>
#include <map>
#include <utility>

std::pair<int,int> solve(int n, int k) {

    std::map<int,int> status;
    status[n] = 1;

    while (true) {
        std::map<int,int> next;
        for (std::map<int,int>::reverse_iterator it = status.rbegin(); it != status.rend(); it++) {
            int x = it->first;
            if (k <= it->second) {
                return std::pair<int,int>(x/2, (x-1)/2);
            }
            k -= it->second;
            next[x/2] += it->second;
            next[(x-1)/2] += it->second;
        }
        status = next;
    }

    return std::pair<int,int>(0,0);
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        int N, K;
        std::cin >> N >> K;
        std::pair<int,int> ans = solve(N, K);
        std::cout << "Case #" << t << ": " << ans.first << " " << ans.second << std::endl;
    }

    return 0;
}

