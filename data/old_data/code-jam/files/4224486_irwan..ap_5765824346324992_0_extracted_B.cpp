#include <iostream>
#include <vector>
using namespace std;

int solve(const vector<int> times, int n) {
    int barberCount = times.size();
    // Binary search to find the exact time t that my hair cut started
    long long int t_min = 0;
    long long int t_max = 1000000000000000000LL;
    int z = 0;
    while (true) {
        long long int t_try = (t_min + t_max) / 2;
        z++;
        if (z == 100) {
            break;
        }
        long long int startedBefore = 0;
        for (int i = 0; i < barberCount; i++) {
            startedBefore += (t_try + times[i]-1) / times[i];
        }
        if (startedBefore >= n) {
            t_max = t_try;
            continue;
        }
        for (int i = 0; i < barberCount; i++) {
            if (t_try % times[i] == 0) {
                startedBefore++;
                if (startedBefore == n) {
                    return i+1;
                }
            }
        }
        t_min = t_try;
    }

    return -1;
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int B, N;
        cin >> B >> N;
        vector<int> m(B);
        for (int i = 0; i < B; i++) {
            cin >> m[i];
        }
        cout << "Case #" << t << ": " << solve(m, N) << endl;
    }

    return 0;
}

