#include <iostream>
#include <string>
#include <vector>
using namespace std;

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int S;
        cin >> S;
        string str;
        cin >> str;
        vector<int> count(S+1);
        for (int i = 0; i <= S; i++) {
            count[i] = str[i]-'0';
        }
        int totalStanding = 0;
        int ans = 0;
        totalStanding += count[0];
        for (int i = 1; i <= S; i++) {
            if (totalStanding < i) {
                ans += i - totalStanding;
                totalStanding = i;
            }
            totalStanding += count[i];
        }
        cout << "Case #" << t << ": " << ans << endl;
    }

    return 0;
}

