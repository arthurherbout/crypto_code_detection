#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

long long solve_case();

int main() {
    ios::sync_with_stdio(false);
    int num_cases;
    cin >> num_cases;
    for (int i=1; i<=num_cases; i++) {
        auto ans = solve_case();
        cout << "Case #" << i << ": ";
        if (ans < 0)
            cout << "CHEATERS!\n";
        else
            cout << ans << '\n';
    }
    return 0;
}

long long solve_case() {
    int n, d;
    cin >> n >> d;
    assert(n != 1);
    assert(d <= n/2);
    vector<long long> data(n);
    for (auto &x: data)
        cin >> x;

    int max_open_idx = 0;
    while ((1 << (max_open_idx+1)) <= d)
        max_open_idx++;
    assert((1 << max_open_idx) == d);

    int max_cycle_idx = max_open_idx + 1;
    int max_cycle = (1 << max_cycle_idx);
    for (int i=0; i<n-max_cycle; i++)
        if (data[i] != data[i+max_cycle])
            return -1;

    long long ans = 0;
    for (int curr_open_idx = max_open_idx; curr_open_idx>=0; curr_open_idx--) {
        int curr_open = (1 << curr_open_idx);
        vector<long long> diffs(curr_open);
        for (int i=0; i<curr_open; i++)
            diffs[i] = data[i+curr_open] - data[i];
        for (int i=0; i<curr_open; i++) {
            long long val;
            if (i == 0)
                val = diffs[0] + diffs[curr_open-1];
            else
                val = diffs[i] - diffs[i-1];
            //cout << i << ' ' << val << '\n';
            if (val % 2 != 0)
                return -1;
            val /= 2;
            if (val == 0)
                continue;
            //cout << "inc\n";
            ans += abs(val);
            for (int j=0; j<i; j++)
                data[j] -= val;
        }
    }
    return ans;
}
