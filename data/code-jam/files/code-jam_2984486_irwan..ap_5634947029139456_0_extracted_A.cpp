#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

void flip(char& c) {
    if (c == '0') {
        c = '1';
    } else {
        c = '0';
    }
}

int findMinFlips(vector<string> outlets, vector<string> devices, int deviceToMatch) {
    int N = outlets.size();
    int L = outlets[0].length();
    vector<bool> needFlip(L, false);
    int flipCount = 0;
    for (int i = 0; i < L; i++) {
        if (outlets[0][i] != devices[deviceToMatch][i]) {
            needFlip[i] = true;
            flipCount++;
        }
    }
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < L; j++) {
            if (needFlip[j]) {
                flip(outlets[i][j]);
            }
        }
    }
    sort(outlets.begin(), outlets.end());
    sort(devices.begin(), devices.end());
    if (outlets != devices) {
        return 1000;
    }
    return flipCount;
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int N, L;
        cin >> N >> L;
        vector<string> chargingOutlets(N);
        for (int i = 0; i < N; i++) {
            cin >> chargingOutlets[i];
        }
        vector<string> devices(N);
        for (int i = 0; i < N; i++) {
            cin >> devices[i];
        }

        int minFlips = 1000;
        for (int deviceToMatch = 0; deviceToMatch < N; deviceToMatch++) {
            minFlips = min(minFlips, findMinFlips(chargingOutlets, devices, deviceToMatch));
        }
        cout << "Case #" << t << ": ";
        if (minFlips > L) {
            cout << "NOT POSSIBLE" << endl;
        } else {
            cout << minFlips << endl;
        }
    }

    return 0;
}

