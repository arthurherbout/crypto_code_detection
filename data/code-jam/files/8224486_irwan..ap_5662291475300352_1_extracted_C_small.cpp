#include <algorithm>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

struct Runner {
    int initialPosition;
    double speed;
    Runner(): initialPosition(0), speed(1.0) {}
    Runner(int x, double y): initialPosition(x), speed(y) {}
};

bool compareByFinishTime(const Runner& a, const Runner& b) {
    double ta = (360-a.initialPosition)/a.speed;
    double tb = (360-b.initialPosition)/b.speed;
    return ta > tb;
}

int solve(vector<Runner> runners) {

    int n = runners.size();
    int ans = 10;

    sort(runners.begin(), runners.end(), compareByFinishTime);
    for (int i = 0; i < n; i++) {
        int deerCross = i;
        int humanCross = 0;
        double t = (360-runners[i].initialPosition)/runners[i].speed;
        for (int j = i+1; j < n; j++) {
            double humanPosition = runners[j].initialPosition + runners[j].speed*t - 360;
            humanCross += int(floor(humanPosition/360));
        }
        ans = min(ans, deerCross + humanCross);
    }

    return ans;
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int N;
        cin >> N;
        vector<int> D(N), H(N), M(N);
        for (int i = 0; i < N; i++) {
            cin >> D[i] >> H[i] >> M[i];
        }
        vector<Runner> runners;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < H[i]; j++) {
                runners.push_back(Runner(D[i], 360.0/(M[i]+j)));
            }
        }
        cout << "Case #" << t << ": " << solve(runners) << endl;
    }

    return 0;
}

