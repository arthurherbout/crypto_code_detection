#include <iostream>
#include <vector>
using namespace std;

int solve(const vector<int>& initial, const vector<int>& count, const vector<int>& fastest) {
    int N = initial.size();
    if (N == 1 && count[0] == 1) {
        return 0;
    }

    if (N == 1) {
        double faster = 360.0/fastest[0];
        double slower = 360.0/(fastest[0]+1);
        double dst = 360 - initial[0];
        double slowerTime = dst/slower;
        double fasterPos = initial[0] + slowerTime * faster;
        if (fasterPos < 720) {
            return 0;
        }
        return 1;
    }

    double dst1 = 360 - initial[0];
    double dst2 = 360 - initial[1];
    double spd1 = 360.0/fastest[0];
    double spd2 = 360.0/fastest[1];
    double tm1 = dst1 / spd1;
    double tm2 = dst2 / spd2;
    double fasterPos;
    if (tm1 < tm2) {
        fasterPos = initial[0] + tm2 * spd1;
    } else {
        fasterPos = initial[1] + tm1 * spd2;
    }
    if (fasterPos < 720) {
        return 0;
    }
    return 1;
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
        cout << "Case #" << t << ": " << solve(D, H, M) << endl;
    }

    return 0;
}

