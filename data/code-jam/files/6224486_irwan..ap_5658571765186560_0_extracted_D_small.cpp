#include <algorithm>
#include <iostream>
using namespace std;

bool alwaysPossible(int x, int r, int c) {
    if (x == 1) {
        return true;
    }
    if (x == 2) {
        return (r*c % 2 == 0);
    }
    if (x == 3) {
        if (r*c % 3 != 0) {
            return false;
        }
        return min(r, c) > 1;
    }
    if (x == 4) {
        if (r*c % 4 != 0) {
            return false;
        }
        return min(r, c) > 2;
    }
    return false;
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int X, R, C;
        cin >> X >> R >> C;
        cout << "Case #" << t << ": ";
        if (alwaysPossible(X, R, C)) {
            cout << "GABRIEL";
        } else {
            cout << "RICHARD";
        }
        cout << endl;
    }

    return 0;
}

