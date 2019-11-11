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

int getpow(int x, int pow) {
    if (pow == 0) return 1;
    if (pow == 1) return x;
    int halfpow = getpow(x, pow/2);
    if (pow % 2 == 0) return getmult(halfpow, halfpow);
    else return getmult(halfpow, getmult(halfpow, x));
}

int main() {
    int totaltc;
    cin >> totaltc;
    int tc = 1;
    long long L, X;
    string line;
    int C[10000];
    while (cin >> L >> X) {
        cin >> line;
        string tmp = line;
        C[0] = getval(tmp[0]);
        for (int i = 1; i < tmp.length(); i++) {
            C[i] = getmult(C[i-1], getval(tmp[i]));
        }
        bool isGood = false;
         // For large input
        if (L > 1) {
            int have = getpow(C[tmp.length()-1], X);

            if (have == -1) { // k is satisfied!
                int firstcut = -1;
                static int vis[20];
                memset(vis, 0, sizeof(vis));
                int cm = 1;
                long long loopcount = 0;
                for (int i = 0; loopcount < X && firstcut == -1; i++) { // find i
                    cm = getmult(cm, getval(tmp[i%tmp.length()]));
                    //cout << i << " loop " << loopcount << endl;
                    if (cm == 2) firstcut = i;
                    if ((i%tmp.length()) == tmp.length()-1) {
                        //cout << "visited " << cm << endl;
                        if (vis[cm+10]) break;
                        vis[cm+10] = 1;
                        loopcount++;
                    }
                }
                if (firstcut != -1) {
                    //cout << "Found i at " << firstcut << endl;
                    memset(vis, 0, sizeof(vis));
                    int secondcut = firstcut;
                    for (int i = firstcut+1; loopcount < X && secondcut == firstcut; i++) { // find j (i*j = k)
                        cm = getmult(cm, getval(tmp[i%tmp.length()]));
                        if (cm == 4) secondcut = i, isGood = true;
                        if ((i%tmp.length()) == tmp.length()-1) {
                            if (vis[cm+10]) break;
                            vis[cm+10] = 1;
                            loopcount++;
                        }
                    }
                }
            }
        }
        /* //AC small input
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
        if (C[tmp.length()-1] == -1) { // i*j*k = -1
            int firstcut = -1;
            for (int i = 0; i < tmp.length() && firstcut == -1; i++) if (C[i] == 2) firstcut = i; // find i
            if (firstcut != -1) {
                int secondcut = firstcut;
                for (int i = firstcut+1; i < tmp.length(); i++) if (C[i] == 4) secondcut = i, isGood = true; // find j (i*j = k)
            }
        }
*/
        cout << "Case #" << tc++ << ": ";
        if (isGood) {
            cout << "YES" << endl;
        } else {
            cout << "NO" << endl;
        }
    }
    return 0;
}
