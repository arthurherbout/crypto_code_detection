#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct tree {
    int N;
    int root;
    vector<vector<int>> children;
    vector<int> parent;
    vector<int> totalNode;
};

void findTotalNode(tree& t, int node) {
    t.totalNode[node] = 1;
    for (size_t i = 0; i < t.children[node].size(); i++) {
        int childNode = t.children[node][i];
        findTotalNode(t, childNode);
        t.totalNode[node] += t.totalNode[childNode];
    }
}

tree constructTree(const vector<vector<int>>& adjacencyList, int root) {
    
    tree ans;
    ans.N = adjacencyList.size();
    ans.root = root;
    ans.children.resize(ans.N);
    ans.parent.resize(ans.N);
    ans.totalNode.resize(ans.N);

    vector<bool> visited(ans.N, false);
    queue<int> q;
    q.push(root);
    visited[root] = true;
    while (!q.empty()) {
        int next = q.front();
        q.pop();
        for (size_t i = 0; i < adjacencyList[next].size(); i++) {
            int v = adjacencyList[next][i];
            if (visited[v]) {
                continue;
            }
            ans.children[next].push_back(v);
            ans.parent[v] = next;
            visited[v] = true;
            q.push(v);
        }
    }

    findTotalNode(ans, ans.root);

    return ans;
}

int makeBinaryTree(tree& t, int node) {
    if (t.children[node].size() == 0) {
        return 0;
    }
    if (t.children[node].size() == 1) {
        return t.totalNode[t.children[node][0]];
    }
    if (t.children[node].size() == 2) {
        int ans = 0;
        for (size_t i = 0; i < t.children[node].size(); i++) {
            ans += makeBinaryTree(t, t.children[node][i]);
        }
        return ans;
    }
    vector<int> cost(t.children[node].size());
    for (size_t i = 0; i < t.children[node].size(); i++) {
        cost[i] = makeBinaryTree(t, t.children[node][i]);
    }
    int ans = 1000;
    for (size_t i = 0; i < t.children[node].size(); i++) {
        for (size_t j = i+1; j < t.children[node].size(); j++) {
            int childi = t.children[node][i];
            int childj = t.children[node][j];
            int c = 0;
            c = t.totalNode[node] - 1 - t.totalNode[childi] - t.totalNode[childj] + cost[i] + cost[j];
            ans = min(ans, c);
        }
    }
    return ans;
}

int findMinimumAssumingRoot(const vector<vector<int>>& adjacencyList, int root) {

    tree t = constructTree(adjacencyList, root);
    return makeBinaryTree(t, root);
}

int main() {

    int T;
    cin >> T;

    for (int t = 1; t <= T; t++) {
        int N;
        cin >> N;
        vector<vector<int>> adjacencyList(N);
        for (int i = 0; i < N-1; i++) {
            int a, b;
            cin >> a >> b;
            a--;
            b--;
            adjacencyList[a].push_back(b);
            adjacencyList[b].push_back(a);
        }
        int ans = N+1;
        for (int i = 0; i < N; i++) {
            int needToDelete = findMinimumAssumingRoot(adjacencyList, i);
            if (needToDelete < ans) {
                ans = needToDelete;
            }
        }
        cout << "Case #" << t << ": " << ans << endl;
    }

    return 0;
}

