#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

int T, maxv;

bool check(const vector<int>& perm, const vector<int>& src) {
    vector<int> sprm;
    for (int i = 0; i < perm.size(); ++i) {
        sprm.push_back(src[perm[i]]);
    }
    if (is_sorted(sprm.begin(), sprm.end())) {
        return true;
    };
    int mxid = 0;
    while (sprm[mxid] < sprm[mxid + 1]) ++mxid;
    for (int id = mxid + 1; id < src.size(); ++id) {
        if (sprm[id] > sprm[id - 1]) {
            return false;
        }
    }
    return true;
}

int main() {
    scanf("%d", &T);
    for (int _ = 0; _ < T; ++_) {
        int N; scanf("%d", &N);
        vector<int> src(N);
        for (int i = 0; i < N; ++i) scanf("%d", &src[i]);
        maxv = src[0];
        for (int i = 0; i < src.size(); ++i) maxv = max(maxv, src[i]);
        vector<int> perm;
        for (int i = 0; i < N; ++i)
            perm.push_back(i);
        int minr = N * (N - 1) / 2;
        do {
            if (check(perm, src)) {
                int whl = 0;
                for (int i = 0; i < perm.size(); ++i) {
                    whl += (perm[i] > i) ? (perm[i] - i) : (i - perm[i]);
                }
                whl /= 2;
                minr = min(minr, whl);
            }
        } while (next_permutation(perm.begin(), perm.end()));
        cout << "Case #" << _  + 1 << ": " << minr << endl;
    }
    return 0;
}
