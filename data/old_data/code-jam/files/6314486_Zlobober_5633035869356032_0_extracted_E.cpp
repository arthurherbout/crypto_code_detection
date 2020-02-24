#include <cstdio>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

const int P = 600500;

vector<pair<int, int>> premade[P];

map<vector<vector<pair<int, int>>>, bool> cache;

bool go(vector<vector<pair<int, int>>> stacks) {
    auto it = cache.find(stacks);
    if (it != cache.end()) {
        return it->second;
    }
    auto& res = cache[stacks];
    bool term = true;
    for (auto& stack : stacks) {
        term &= stack.size() <= 1;
    }
    if (term) {
        return res = true;
    }
    for (int i = 0; i < (int)stacks.size(); i++) {
        if (stacks[i].empty()) {
            for (int j = 0; j < (int)stacks.size(); j++) {
                if (stacks[j].size() >= 2) {
                    auto nstacks = stacks;
                    nstacks[i].push_back(nstacks[j].back());
                    nstacks[j].pop_back();
                    if (go(nstacks)) {
                        return res = true;
                    }
                }
            }
        }
    }
    for (int i = 0; i < (int)stacks.size(); i++) {
        if (stacks[i].empty())
            continue;
        for (int j = 0; j < (int)stacks.size(); j++) {
            if (stacks[j].empty())
                continue;
            if (i == j)
                continue;
            if (stacks[i].back().second == stacks[j].back().second && 
                stacks[i].back().first <= stacks[j].back().first)
            {
                auto nstacks = stacks;
                nstacks[i].pop_back();
                if (go(nstacks)) {
                    return res = true;
                }
            }
        }
    }
    return res = false;
}

void solve(int cs) {
    int n;
    int unused;
    scanf("%d", &n);
    scanf("%d", &unused);
    vector<vector<pair<int, int>>> stacks;
    for (int i = 0; i < n; i++) {
        int p;
        scanf("%d", &p);
        stacks.emplace_back(premade[p]);
    }
    cache.clear();
    if (go(stacks)) {
        printf("Case #%d: POSSIBLE\n", cs);
    } else {
        printf("Case #%d: IMPOSSIBLE\n", cs);
    }
    fprintf(stderr, "cache size = %d\n", (int)cache.size());
}

int main() {
    int p;
    scanf("%d", &p);
    for (int i = 0; i < p; i++) {
        int c;
        scanf("%d", &c);
        premade[i].resize(c);
        for (int j = 0; j < c; j++) {
            scanf("%d %d", &premade[i][j].first, &premade[i][j].second);
        }
        reverse(premade[i].begin(), premade[i].end());
    }


    int T;
    scanf("%d", &T);
    for (int cs = 0; cs < T; cs++) {
        solve(cs + 1);
        fprintf(stderr, "%d\n", cs);
    }
}
