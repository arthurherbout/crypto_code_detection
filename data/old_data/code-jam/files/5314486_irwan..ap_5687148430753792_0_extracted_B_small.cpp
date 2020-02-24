#include <iostream>
#include <utility>
#include <vector>

struct ticket {
    int seat;
    int customer;
};

std::pair<int,int> solve(int n, int c, std::vector<ticket> tickets) {

    std::vector<int> c1(n+1), c2(n+1);
    int t1 = 0, t2 = 0;
    int m = tickets.size();
    for (int i = 0; i < m; i++) {
        if (tickets[i].customer == 1) {
            t1++;
            c1[tickets[i].seat]++;
        } else {
            t2++;
            c2[tickets[i].seat]++;
        }
    }

    int min_rides = std::max(c1[1]+c2[1], std::max(t1, t2));
    int min_promotions = 0;
    for (int i = 2; i <= n; i++) {
        if (c1[i] + c2[i] > min_rides) {
            min_promotions += c1[i] + c2[i] - min_rides;
        }
    }

    return std::pair<int,int>(min_rides, min_promotions);
}

int main() {

    int T;
    std::cin >> T;
    for (int t = 1; t <= T; t++) {
        int N, C, M;
        std::cin >> N >> C >> M;
        std::vector<ticket> tickets(M);
        for (int i = 0; i < M; i++) {
            std::cin >> tickets[i].seat >> tickets[i].customer;
        }
        std::pair<int,int> ans = solve(N, C, tickets);
        std::cout << "Case #" << t << ": ";
        std::cout << ans.first << " " << ans.second << std::endl;
    }

    return 0;
}

