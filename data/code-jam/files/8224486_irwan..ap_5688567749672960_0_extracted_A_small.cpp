#include <iostream>
#include <queue>
#include <vector>
using namespace std;

int reverseNumber(int x) {
    if (x == 0) {
        return 0;
    }
    vector<int> digits;
    while (x > 0) {
        digits.push_back(x%10);
        x /= 10;
    }
    int ans = 0;
    for (size_t i = 0; i < digits.size(); i++) {
        ans = ans*10 + digits[i];
    }
    return ans;
}

int solve(int N) {
    vector<int> best(1000001, 2000000000);
    best[1] = 1;
    queue<int> q;
    q.push(1);

    while (!q.empty()) {
        int next = q.front();
        q.pop();
        if (next == N) {
            return best[next];
        }
        int n1 = next + 1;
        int n2 = reverseNumber(next);
        if (best[next] + 1 < best[n1]) {
            best[n1] = best[next] + 1;
            q.push(n1);
        }
        if (best[next] + 1 < best[n2]) {
            best[n2] = best[next] + 1;
            q.push(n2);
        }
    }

    return N;
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int N;
        cin >> N;
        cout << "Case #" << t << ": " << solve(N) << endl;
    }

    return 0;
}

