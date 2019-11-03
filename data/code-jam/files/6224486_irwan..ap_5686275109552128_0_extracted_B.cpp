#include <iostream>
#include <vector>
using namespace std;

bool possible(const vector<int>& a, int limit) {
    int n = a.size();
    for (int i = 1; i <= limit; i++) {
        int moveAvailable = limit-i;
        int moveRequired = 0;
        for (int j = 0; j < n; j++) {
            moveRequired += (a[j]+i-1)/i - 1;
        }
        if (moveRequired <= moveAvailable) {
            return true;
        }
    }
    return false;
}

int solve(const vector<int>& a) {

    int hi = 1000;
    int low = 1;
    int z = 0;
    while (true) {
        z++;
        int x = (hi + low) / 2;
        if (z == 100) {
            break;
        }
        bool px = possible(a, x);
        bool px1 = possible(a, x-1);
        if (px && !px1) {
            return x;
        }
        if (!px) {
            low = x + 1;
        }
        if (px1) {
            hi = x - 1;
        }
    }
    return -1;
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int D;
        cin >> D;
        vector<int> a(D);
        for (int i = 0; i < D; i++) {
            cin >> a[i];
        }
        cout << "Case #" << t << ": " << solve(a) << endl;
    }

    return 0;
}

