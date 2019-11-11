#include <iostream>
#include <map>
#include <utility>

std::pair<long long int,long long int> solve(long long int n, long long int k) {

    std::map<long long int,long long int> status;
    status[n] = 1;

    while (true) {
        std::map<long long int,long long int> next;
        for (std::map<long long int,long long int>::reverse_iterator it = status.rbegin(); it != status.rend(); it++) {
            long long int x = it->first;
            if (k <= it->second) {
                return std::pair<long long int,long long int>(x/2, (x-1)/2);
            }
            k -= it->second;
            next[x/2] += it->second;
            next[(x-1)/2] += it->second;
        }
        status = next;
    }

    return std::pair<long long int,long long int>(0,0);
}

int main() {

    int T;
    std::cin >> T;

    for (int t = 1; t <= T; t++) {
        long long int N, K;
        std::cin >> N >> K;
        std::pair<long long int,long long int> ans = solve(N, K);
        std::cout << "Case #" << t << ": " << ans.first << " " << ans.second << std::endl;
    }

    return 0;
}

