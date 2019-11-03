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

bool solve(const string& s) {
    int n = s.length();
    vector<int> leftMults(n);
    vector<int> rightMults(n);
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
    for (int i = 0; i < n-2; i++) {
        if (leftMults[i] == I) {
            for (int j = i+1; j < n-1; j++) {
                if (divideLeft(leftMults[j], leftMults[i]) == J) {
                    if (rightMults[j+1] == K) {
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
        int L, X;
        string s;
        cin >> L >> X;
        cin >> s;
        string str;
        for (int i = 0; i < X; i++) {
            str += s;
        }
        cout << "Case #" << t << ": ";
        if (solve(str)) {
            cout << "YES";
        } else {
            cout << "NO";
        }
        cout << endl;
    }

    return 0;
}

