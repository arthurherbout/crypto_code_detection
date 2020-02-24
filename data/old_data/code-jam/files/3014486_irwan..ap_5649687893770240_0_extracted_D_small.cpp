#include <algorithm>
#include <iostream>
#include <map>
#include <set>
#include <string>
using namespace std;

int countNode(const vector<string>& vs) {
    set<string> s;
    int n = vs.size();
    for (int i = 0; i < n; i++) {
        int l = vs[i].length();
        for (int j = 0; j <= l; j++) {
            s.insert(vs[i].substr(0, j));
        }
    }
    return s.size();
}

bool next(vector<int>& s, int N) {
    int M = s.size();
    int i = M-1;
    while (i >= 0 && s[i] == N-1) {
        i--;
    }
    if (i == -1) {
        return false;
    }
    s[i]++;
    for (int j = i+1; j < M; j++) {
        s[j] = 0;
    }
    return true;
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int M, N;
        cin >> M >> N;
        vector<string> s(M);
        for (int i = 0; i < M; i++) {
            cin >> s[i];
        }
        map<int,int> ways;
        int worst = 0;
        vector<int> assign(M, 0);
        do {
            vector<vector<string>> strings(N);
            for (int i = 0; i < M; i++) {
                strings[assign[i]].push_back(s[i]);
            }

            int trieCount = 0;
            for (int i = 0; i < N; i++) {
                int tr = countNode(strings[i]);
                trieCount += tr;
            }

            worst = max(worst, trieCount);
            ways[trieCount]++;
        } while (next(assign, N));
        int wayWorst = ways[worst];
        cout << "Case #" << t << ": " << worst << " " << wayWorst << endl;
    }

    return 0;
}

