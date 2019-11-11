#include <iostream>
#include <vector>
#include <cstdio>
using namespace std;

double solve(double desiredVolume, double desiredTemperature, const vector<double>& rate, const vector<double>& temperature) {
    int N = rate.size();
    if (N == 1) {
        if (desiredTemperature != temperature[0]) {
            return -1;
        }
        return desiredVolume / rate[0];
    }
    if (desiredTemperature == temperature[0] && desiredTemperature == temperature[1]) {
        return desiredVolume/(rate[0]+rate[1]);
    }
    double v0 = desiredVolume * (desiredTemperature-temperature[1])/(temperature[0]-temperature[1]);
    double v1 = desiredVolume * (desiredTemperature-temperature[0])/(temperature[1]-temperature[0]);
    if (v0 < 0 || v1 < 0) {
        return -1;
    }
    return max(v0/rate[0], v1/rate[1]);
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int N;
        double desiredVolume;
        double desiredTemperature;
        cin >> N >> desiredVolume >> desiredTemperature;

        vector<double> rate(N);
        vector<double> temperature(N);
        for (int i = 0; i < N; i++) {
            cin >> rate[i] >> temperature[i];
        }

        double ans = solve(desiredVolume, desiredTemperature, rate, temperature);
        cout << "Case #" << t << ": ";
        if (ans < 0) {
            cout << "IMPOSSIBLE" << endl;
        } else {
            printf("%.6lf\n", ans);
        }
    }

    return 0;
}

