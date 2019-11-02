#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

bool check_ok_one(int N, const std::vector<std::string>& able, const std::vector<int>& work_order, const std::vector<int>& machine_chosen) {
    std::vector<bool> taken(N, false);
    for (int i = 0; i < N; i++) {
        int worker = work_order[i];
        int machine = machine_chosen[i];
        int count = 0;
        int current_machine = 0;
        bool found_match = false;
        for (int j = 0; j < N*N; j++) {
            if (!taken[current_machine] && able[worker][current_machine] == '1') {
                if (machine == count) {
                    taken[current_machine] = true;
                    found_match = true;
                    break;
                }
                count++;
            }
            current_machine++;
            current_machine %= N;
        }
        if (!found_match) {
            return false;
        }
    }
    return true;
}

bool next_state(std::vector<int>& current, int N) {
    current[0]++;
    for (int i = 1; i < N; i++) {
        current[i] += current[i-1]/N;
        current[i-1] %= N;
    }
    return current[N-1] != N;
}

bool check_ok(int N, const std::vector<std::string>& able) {
    std::vector<int> work_order;
    for (int i = 0; i < N; i++) {
        work_order.push_back(i);
    }
    do {
        std::vector<int> machine_chosen(N, 0);
        do {
            bool ok = check_ok_one(N, able, work_order, machine_chosen);
            if (!ok) {
                return false;
            }
        } while (next_state(machine_chosen, N));
    } while (next_permutation(work_order.begin(), work_order.end()));
    return true;
}

int solve(int N, std::vector<std::string> able) {
    int bf = N*N;
    int ans = bf+1;
    for (int bit = 0; bit < (1<<bf); bit++) {
        int cost = 0;
        std::vector<std::string> new_able = able;
        bool redundant = false;
        for (int j = 0; j < bf; j++) {
            if (bit & (1<<j)) {
                if (new_able[j/N][j%N] == '1') {
                    redundant = true;
                    break;
                }
                cost++;
                new_able[j/N][j%N] = '1';
            }
        }
        if (redundant) {
            continue;
        }
        if (check_ok(N, new_able)) {
            ans = std::min(ans, cost);
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
        std::vector<std::string> able(N);
        for (int i = 0; i < N; i++) {
            std::cin >> able[i];
        }
        int ans = solve(N, able);
        std::cout << "Case #" << t << ": " << ans << std::endl;
    }

    return 0;
}

