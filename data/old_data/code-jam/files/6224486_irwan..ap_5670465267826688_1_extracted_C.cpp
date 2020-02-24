#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define ONE 0
#define I 1
#define J 2
#define K 3
#define N_ONE 4
#define N_I 5
#define N_J 6
#define N_K 7

const vector<vector<int>> table = {
    {ONE, I, J, K},
    {I, N_ONE, K, N_J},
    {J, N_K, N_ONE, I},
    {K, J, N_I, N_ONE},
};

// Find a * b
int multiply(int a, int b) {
    int sgn = +1;
    if (a >= 4) {
        sgn *= -1;
        a -= 4;
    }
    if (b >= 4) {
        sgn *= -1;
        b -= 4;
    }
    int c = table[a][b];
    if (c >= 4) {
        sgn *= -1;
        c -= 4;
    }
    if (sgn < 0) {
        sgn *= -1;
        c += 4;
    }
    return c;
}

// Find x so that b * x = a
int divideLeft(int a, int b) {
    for (int i = 0; i < 8; i++) {
        if (multiply(b, i) == a) {
            return i;
        }
    }
    // impossible
    return -1;
}

int val(char c) {
    switch (c) {
        case 'i':
            return I;
        case 'j':
            return J;
        case 'k':
            return K;
    }
    return -1;
}

bool solve(long long int rep, const string& s) {

    int n = s.length();
    vector<int> leftMults(n);
    vector<int> rightMults(n+1);
    int M = ONE;
    for (int i = 0; i < n; i++) {
        M = multiply(M, val(s[i]));
        leftMults[i] = M;
    }
    M = ONE;
    for (int j = n-1; j >= 0; j--) {
        M = multiply(val(s[j]), M);
        rightMults[j] = M;
    }
    rightMults[n] = ONE;

    int oneCycle = leftMults[n-1];
    vector<int> cycles(5);
    cycles[0] = ONE;
    for (int i = 1; i <= 4; i++) {
        cycles[i] = multiply(cycles[i-1], oneCycle);
    }
    for (int i = 0; i < 4; i++) {
        if (rep-i-1 < 0) {
            break;
        }
        for (int j = 0; j < n; j++) {
            int left = multiply(cycles[i], leftMults[j]);
            if (left != I) {
                continue;
            }
            for (int k = j+1; k < n; k++) {
                int mid = divideLeft(leftMults[k], leftMults[j]);
                int right = multiply(rightMults[k+1], cycles[(rep-i-1)%4]);
                if (mid == J && right == K) {
                    return true;
                }
            }
            for (int l = 0; l < 4; l++) {
                if(rep-i-1-l-1 < 0) {
                    break;
                }
                for (int k = 0; k < n; k++) {
                    int mid = multiply(rightMults[j+1], multiply(cycles[l], leftMults[k]));
                    int right = multiply(rightMults[k+1], cycles[(rep-i-1-l-1)%4]);
                    if (mid == J && right == K) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int L;
        long long int X;
        string s;
        cin >> L >> X;
        cin >> s;
        cout << "Case #" << t << ": ";
        if (solve(X, s)) {
            cout << "YES";
        } else {
            cout << "NO";
        }
        cout << endl;
    }

    return 0;
}

