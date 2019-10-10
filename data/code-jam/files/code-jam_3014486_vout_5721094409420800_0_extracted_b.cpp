#include <cstdio>
#include <vector>
#include <set>
#include <queue>

using namespace std;

bool check(vector<int> &v) {
    bool changed = false;
    for (int i = 0; i < v.size() - 1; ++i) {
        if (changed) {
            if (v[i] < v[i + 1]) {
                return false;
            }
        } else if (v[i] > v[i + 1]) {
            changed = true;
        }
    }
    return true;
}

int main() {
    int T;
    scanf("%d", &T);
    for (int re = 1; re <= T; ++re) {
        int n;
        scanf("%d", &n);
        vector<int> v(n);
        for (int i = 0; i < n; ++i) {
            scanf("%d", &v[i]);
        }
        queue<pair<vector<int>, int> > q;
        set<vector<int> > mark;
        q.push(make_pair(v, 0));
        mark.insert(v);
        int ans = -1;
        while (!q.empty()) {
            vector<int> v = q.front().first;
            int step = q.front().second;
            q.pop();
            if (check(v)) {
                ans = step;
                break;
            }
            for (int i = 1; i < v.size(); ++i) {
                swap(v[i], v[i - 1]);
                if (!mark.count(v)) {
                    mark.insert(v);
                    q.push(make_pair(v, step + 1));
                }
                swap(v[i], v[i - 1]);
            }
        }
        printf("Case #%d: %d\n", re, ans);
    }
    return 0;
}