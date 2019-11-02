#include <bits/stdc++.h>
using namespace std;

int M[5][5] = {{},{0,1,2,3,4},{0,2,-1,4,-3},{0,3,-4,-1,2},{0,4,3,-2,-1}};

int getval(char ch) {
    if (ch == 'i') return 2;
    if (ch == 'j') return 3;
    else return 4;
}

int getmult(int x, int y) {
    if (x < 0 && y > 0) return -M[-x][y];
    else if (x > 0 && y < 0) return -M[x][-y];
    else if (x < 0 && y < 0) return M[-x][-y];
    else return M[x][y];
}

int main() {
    int totaltc;
    cin >> totaltc;
    int tc = 1;
    int L, X;
    string line;
    int C[10000];
    while (cin >> L >> X) {
        cin >> line;
        string tmp = line;
        for (int i = 0; i < X-1; i++) tmp += line;
        //cout << tmp << endl;
        memset(C, 0, sizeof(C));
        C[0] = getval(tmp[0]);
        //cout << C[0];
        for (int i = 1; i < tmp.length(); i++) {
            C[i] = getmult(C[i-1], getval(tmp[i]));
            //cout << C[i];
        }
        //cout << endl;
        bool isGood = false;
        if (C[tmp.length()-1] == -1) { // i*j*k = -1
            int firstcut = -1;
            for (int i = 0; i < tmp.length() && firstcut == -1; i++) if (C[i] == 2) firstcut = i; // find i
            if (firstcut != -1) {
                int secondcut = firstcut;
                for (int i = firstcut+1; i < tmp.length(); i++) if (C[i] == 4) secondcut = i, isGood = true; // find j (i*j = k)
            }
        }
        cout << "Case #" << tc++ << ": ";
        if (isGood) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
    return 0;
}
