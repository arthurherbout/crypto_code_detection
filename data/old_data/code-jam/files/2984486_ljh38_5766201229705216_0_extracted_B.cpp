#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>
#include <limits>
#include <queue>
#include <algorithm>
#include <set>

using namespace std;

struct Node {
    vector<int> Adjacent;

    int Parent;
    int NumChildren;
    int NumBelow;
};

void dfs(int from, vector<Node>& Nodes, int n, vector<int>& BottomUp) {
    Node& N = Nodes[n];
    N.NumChildren = 0;
    N.NumBelow = 0;
    N.Parent = from;
    for (int i = 0; i != N.Adjacent.size(); ++i) {
        int child = N.Adjacent[i];
        if (child != from) {
            dfs(n, Nodes, child, BottomUp);
            ++N.NumChildren;
            N.NumBelow += Nodes[child].NumBelow + 1;
        }
    }

    BottomUp.push_back(n);
}

void reduce(vector<Node>& Nodes, int n, int amount) {
    if (n != -1) {
        Nodes[n].NumBelow -= amount;
        assert(Nodes[n].NumBelow >= 0);

        reduce(Nodes, Nodes[n].Parent, amount);
    }
}

int numToDelete(vector<Node> Nodes, int Root) {
    vector<int> BottomUp;
    dfs(-1, Nodes, Root, BottomUp);

    int totalDeletions = 0;

    for (int i = 0; i != BottomUp.size(); ++i) {
        int N = BottomUp[i];
        Node& node = Nodes[N];

        if (node.NumChildren == 1) {
            // cout << "Bad 1-node " << N +1 << " deleting " << node.NumBelow << endl;
            
            // bad node, must delete everything below it
            totalDeletions += node.NumBelow;
            reduce(Nodes, N, node.NumBelow);
        } else if (node.NumChildren > 2) {
            // bad node, must delete all but 2 children.
            vector<int> childCounts;
            for (int childI = 0; childI != node.Adjacent.size(); ++childI) {
                if (node.Adjacent[childI] == node.Parent) {
                    continue;
                }
                int child = node.Adjacent[childI];
                childCounts.push_back(Nodes[child].NumBelow);
            }
            sort(childCounts.begin(), childCounts.end());
            // keep the 2 children with most nodes below them
            int thisDeletion = 0;
            for (int childI = 0; childI + 2 < childCounts.size(); ++childI) {
                thisDeletion += childCounts[childI] + 1;
            }

            totalDeletions += thisDeletion;
            reduce(Nodes, N, thisDeletion);

            // cout << "Bad >2-node " << N + 1 << " deleting " << thisDeletion << endl;
        }
    }

    return totalDeletions;
}

int main() {
    // ifstream in("sample.in");
    istream& in = cin;

    int C;
    in >> C;
    for (int c = 0; c != C; ++c) {
        int N;
        in >> N;
        vector<Node> nodes(N);
        
        for (int n = 0; n != N - 1; ++n) {
            int X, Y;
            in >> X >> Y;
            nodes[X - 1].Adjacent.push_back(Y - 1);
            nodes[Y - 1].Adjacent.push_back(X - 1);
        }

        int best = numeric_limits<int>::max();
        // cout << "Rooting at 1 " << numToDelete(nodes, 0) << endl;
        for (int root = 0; root != N; ++root) {
            int m = numToDelete(nodes, root);
            // cout << "Rooting at " << root << " requires " << m << endl;
            best = min(m, best);
        }
        cout << "Case #" << (c+1) << ": " << best << endl;
    }
    return 0;
}
