#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

enum {
    GONE = 0,
    DELETED = 1,
    EXISTS = 2,
    ADDED = 3
};

void add(vector<int> &state, const vector< vector<int> > &sons, const vector<int> &father, int node, int &count) {
    if (state[node] != EXISTS)
        return;
    state[node] = ADDED;
    count++;
    for (auto &next : sons[node])
        add(state, sons, father, next, count);
}

void exists(vector<int> &state, const vector< vector<int> > &sons, const vector<int> &father, int node, int& count) {
    if (state[node] != DELETED)
        return;
    state[node] = EXISTS;
    if (!node || state[father[node]] == ADDED)
        add(state, sons, father, node, count);
}

void gone(vector<int> &state, const vector< vector<int> > &sons, const vector<int> &father, int node, int &count) {
    if (state[node] == GONE)
        return;
    if (state[node] == ADDED) {
        --count;
    }
    state[node] = GONE;
    for (auto &next : sons[node])
        gone(state, sons, father, next, count);
}

int main() {
    ios::sync_with_stdio(false);

    int T; cin >> T;
    for (int test = 1; test <= T; ++test) {
        int N, D; cin >> N >> D;

        vector<int> S(N);
        vector<int> M(N);

        int S0, AS, CS, RS; cin >> S0 >> AS >> CS >> RS;
        int M0, AM, CM, RM; cin >> M0 >> AM >> CM >> RM;

        S[0] = S0;
        M[0] = M0;
        for (int i = 1; i < N; ++i) {
            S[i] = (1LL * S[i - 1] * AS + CS) % RS;
            M[i] = (1LL * M[i - 1] * AM + CM) % RM;
        }
        M[0] = -1;

        vector< vector<int> > sons(N);
        for (int i = 1; i < N; ++i) {
            M[i] %= i;
            sons[M[i]].push_back(i);
        }

        vector<int> order(N);
        for (int i = 0; i < N; ++i)
            order[i] = i;
        sort(order.begin(), order.end(), [&](int x, int y) {
            return S[x] > S[y];
        });

        int answer = 0;
        int count = 0;
        int current = 0;
        vector<int> state(N, DELETED);
        for (int i = 0; i < N; ++i) {
            for (; current < N && S[order[current]] >= S[order[i]] - D; ++current)
                exists(state, sons, M, order[current], count);
            if (S[order[i]] >= S[0] && S[order[i]] - D <= S[0])
                answer = max(answer, count);
            gone(state, sons, M, order[i], count);
        }

        cout << "Case #" << test << ": " << answer << "\n";
    }
}
