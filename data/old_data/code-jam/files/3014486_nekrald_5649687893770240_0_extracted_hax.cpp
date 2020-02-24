#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <string>
#include <vector>

using namespace std;
const int CSZ = 26;

struct node {
    int nxt[400];
} trg[4][800];

int sz[4];

void add_string(string s, int tid) {
    for (int i = 0, cnd = 0; i < s.length(); ++i) {
        if (trg[tid][cnd].nxt[s[i]] != -1) {
            cnd = trg[tid][cnd].nxt[s[i]];
        } else {
            memset(trg[tid][sz[tid]].nxt, 255, sizeof(trg[tid][sz[tid]].nxt));
            trg[tid][cnd].nxt[s[i]] = sz[tid];
            cnd = sz[tid];
            ++sz[tid];
        }
    }
}

int T, M, N;
vector<string> in;
vector<vector<int> > msk;

void dfs(vector<int>& cur) {
    if (cur.size() == M) {
        msk.push_back(cur);
        return;
    } else {
        for (int i = 0; i < N; ++i) {
            cur.push_back(i);
            dfs(cur);
            cur.pop_back();
        }
    }
}

int main() {
    scanf("%d", &T);
    for (int _ = 1; _ <= T; ++_) {
        in.clear();
        msk.clear();
        scanf("%d%d", &M, &N);
        for (int i = 0; i < M; ++i) {
            string inp;
            cin >> inp;
            in.push_back(inp);
        }
        vector<int> x;
        dfs(x);
        int max_ans  = 0;
        int cnt = 0;
        for (auto& arg : msk) {
            for (int i = 0; i < 4; ++i) {
                memset(trg[i][0].nxt, 255, sizeof(trg[i][0].nxt));
                memset(sz, 0, sizeof(sz));
            }
            for (int i  = 0; i < arg.size(); ++i) {
                if (!sz[arg[i]])
                    sz[arg[i]] = 1;
                add_string(in[i], arg[i]);
            }
            int cand = 0;
            for (int i = 0; i < N; ++i) {
                cand += sz[i];
            }
            if (cand == max_ans) ++cnt;

            if (cand > max_ans) {
                cnt = 1;
                max_ans = cand;
            }
        }
        cout << "Case #" << _ << ": " << max_ans << " " << cnt << endl;
    }
}
