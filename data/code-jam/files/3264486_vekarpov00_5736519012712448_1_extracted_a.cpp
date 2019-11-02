#include <iostream>
#include <vector>
#include <string>

using namespace std;


int main() {
    //ifstream cin("input1.in");
    //ofstream cout("output.txt");
    int t, n, u1 = 0, j;
    string s;
    cin >> t;
    for (int t1 = 0; t1 < t; t1++) {
        cout << "Case #" << t1 + 1 << ": ";
        cin >> s;
        cin >> n;
        u1 = 0;
        j = 0;
        while (u1 + n <= s.length()) {
            if (s[u1] == '-') {
                j++;
                for (int y = u1; y < u1 + n; y++) {
                    if (s[y] == '+') s[y] = '-';
                    else s[y] = '+';
                }
                //cout << s << endl;
            }
            u1++;
        }
        bool t11 = true;
        //cout << s << endl;
        for (int y = u1; y < s.length(); y++) {
            if (s[y] == '-') t11 = false;
        }
        if (t11) cout << j << endl;
        else cout << "IMPOSSIBLE" << endl;
    }
    return 0;
}
