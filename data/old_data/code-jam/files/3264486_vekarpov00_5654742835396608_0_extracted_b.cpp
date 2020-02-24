#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    //ifstream cin("input.in");
    //ofstream cout("output.txt");
    int t;
    long long n, m;
    cin >> t;
    for (int t1 = 0; t1 < t; t1++) {
        cout << "Case #" << t1 + 1 << ": ";
        cin >> n >> m;
        vector<int> v(n + 2, 0);
        vector<int> v1(n + 2, 0);
        vector<int> v2(n + 2, 0);
        v[0] = 1;
        v[n + 1] = 1;
        int mmina, mminb, ind;
        for (int i = 0; i < m; i++) {
            int pr = 0;
            for (int j = 0; j < n + 1; j++) {
                if (v[j] == 1) {
                    v1[j] = -1e9;
                    pr = 0;
                } else {
                    v1[j] = pr;
                    pr++;
                }
            }
            for (int j = n + 1; j > 0; j--) {
                if (v[j] == 1) {
                    v2[j] = -1e9;
                    pr = 0;
                } else {
                    v2[j] = pr;
                    pr++;
                }
            }
            mmina = -1, mminb = -1, ind = -1;
            for (int j = 1; j < n + 1; j++) {
                if (min(v1[j], v2[j]) > mmina) {
                    mmina = min(v1[j], v2[j]);
                    mminb = max(v1[j], v2[j]);
                    ind = j;
                }
                if (min(v1[j], v2[j]) == mmina && max(v1[j], v2[j]) > mminb) {
                    mmina = min(v1[j], v2[j]);
                    mminb = max(v1[j], v2[j]);
                    ind = j;
                }
            }
            v[ind] = 1;
        }
        cout << mminb << " " << mmina << endl;
    }
    return 0;
}
