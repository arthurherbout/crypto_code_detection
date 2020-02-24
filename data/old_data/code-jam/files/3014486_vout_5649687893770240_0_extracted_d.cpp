#include <cstdio>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

struct Trie {
    struct node {
        char c;
        int p[26];
    };

    int root;
    int size;
    node a[10086];

    void init_node(int i, int c) {
        a[i].c = c;
        memset(a[i].p, -1, sizeof(int) * 26);
    }

    void init() {
        root = 0;
        size = 1;
        init_node(root, -1);
    }

    void insert(const char *s) {
        int ptr = root;
        for (int i = 0; s[i]; ++i) {
            if (a[ptr].p[s[i] - 'A'] == -1) {
                a[ptr].p[s[i] - 'A'] = size;
                init_node(size, s[i] - 'A');
                ++size;
            }
            ptr = a[ptr].p[s[i] - 'A'];
        }
    }
} trie;

long long max_nodes, ans;

void dfs(vector<vector<int> > &vv, vector<string> &s, int i, int m) {
    if (i == m) {
        long long num_nodes = 0;
        for (int j = 0; j < vv.size(); ++j) {
            if (vv[j].size() == 0) {
                return;
            }
            trie.init();
            for (int k = 0; k < vv[j].size(); ++k) {
                trie.insert(s[vv[j][k]].c_str());
            }
            num_nodes += trie.size;
        }
        if (num_nodes == max_nodes) {
            ans++;
        } else if (num_nodes > max_nodes) {
            ans = 1;
            max_nodes = num_nodes;
        }
        return;
    }

    for (int j = 0; j < vv.size(); ++j) {
        vv[j].push_back(i);
        dfs(vv, s, i + 1, m);
        vv[j].pop_back();
    }
}

int main() {
    int T;
    cin >> T;
    for (int re = 1; re <= T; ++re) {
        int m, n;
        cin >> m >> n;
        vector<string> s(m);
        for (int i = 0; i < m; ++i) {
            cin >> s[i];
        }
        vector<vector<int> > vv(n);
        ans = 0;
        max_nodes = -1;
        dfs(vv, s, 0, m);
        cout << "Case #" << re << ": " << max_nodes << ' ' << ans << endl;
    }
    return 0;
}