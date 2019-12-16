#include <iostream>
#include <utility>
#include <vector>

struct ticket {
    int seat;
    int customer;
};

std::pair<int,int> solve(int n, int c, std::vector<ticket> tickets) {
    
    int m = tickets.size();

    std::vector<int> total_tickets_per_position(n+1);
    std::vector<int> total_tickets_per_customer(c+1);
    for (int i = 0; i < m; i++) {
        total_tickets_per_customer[tickets[i].customer]++;
        total_tickets_per_position[tickets[i].seat]++;
    }
    int max_tickets = 0;
    for (int i = 1; i <= c; i++) {
        max_tickets = std::max(max_tickets, total_tickets_per_customer[i]);
    }
    
    int min_rides = (m+n-1)/n;
    min_rides = std::max(min_rides, total_tickets_per_position[1]);
    min_rides = std::max(min_rides, max_tickets);

    int min_promotions = 0;
    for (int i = 1; i <= n; i++) {
        if (total_tickets_per_position[i] > min_rides) {
            min_promotions += total_tickets_per_position[i] - min_rides;
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

