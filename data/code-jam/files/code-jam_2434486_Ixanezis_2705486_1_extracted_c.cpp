#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <cstring>
#include <map>
#include <fstream>
#include <set>
#include <unordered_map>

using namespace std;

vector<string> all;

char curStr[6];

vector<string> calc(const string& start) {
    vector<string> ret;
    for (size_t change=0; change<start.size(); ++change) {
        //cout << "change = " << change;
        string init = start;
        string cur = init;
        for (char put='a'; put <= 'z'; ++put) {
            cur[change] = put;

            for (size_t len = 1; len <= start.size(); ++len) {
                string now = cur.substr(0, len);

                //cout << "now = " << now << endl;
                auto it = lower_bound(all.begin(), all.end(), now);
                while (it != all.end()) {
                    if (it->substr(0, 4) != cur.substr(0, it->size())) break;
                    ret.push_back(*it);
                    ++it;
                }
            }
        }
    }

    sort(ret.begin(), ret.end());
    ret.erase(unique(ret.begin(), ret.end()), ret.end());

    return ret;
}

int dp[4444][7];
const int INF = 0x7f7f7f7f;

string s;
int getAns(int pos, int lastError) {
    lastError = min(lastError, 5);
    if (dp[pos][lastError] != -1)
        return dp[pos][lastError];

    if (size_t(pos) == s.size()) {
        return 0;
    }

    //cout << "getAns " << pos << ' ' << lastError << endl;

    int& ret = dp[pos][lastError];
    ret = INF;

    string cur = s.substr(pos, 4);
    vector<string> put = calc(cur);

    /*
    cout << "string " << cur << " matches: ";
    cout << put.size() << ' ';
    for (const string& match : put)
        cout << match << ' ';

    cout << endl;
    */

    for (const string& can : put) {
        if (pos + can.size() > s.size()) continue;
        vector<int> errors;
        errors.push_back(-lastError);
        for (size_t i=0; i<can.size(); ++i) {
            if (can[i] != s[pos + i]) {
                errors.push_back(i);
            }
        }

        bool ok = true;
        for (size_t i=1; i<errors.size(); i++) {
            if (errors[i] - errors[i-1] <= 4) {
                ok = false;
                break;
            }
        }

        if (!ok) continue;

        ret = min(ret, int(errors.size()) - 1 + getAns(pos + can.size(), can.size() - errors[errors.size() - 1]));
    }

    return ret;
}

int main() {
    ifstream fin("garbled.txt");
    for (string word; fin >> word;)
        all.push_back(word);

    sort(all.begin(), all.end());

    int t; cin >> t;
    for (int e=0; e<t; ++e) {
        cin >> s;
        memset(dp, -1, sizeof dp);

        int ans = getAns(0, 5);

        cout << "Case #" << e+1 << ": " << ans << endl;
    }

    return 0;
}
