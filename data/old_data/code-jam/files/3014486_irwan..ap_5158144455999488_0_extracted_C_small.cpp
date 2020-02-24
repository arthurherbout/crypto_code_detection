#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

#define IN 0
#define OUT 1

int maximumFlowGraph(const vector<vector<pair<int,int>>>& graph) {

    int n = graph.size();
    int source = 0;
    int sink = n-1;

    int flow = 0;
    vector<map<int,int>> residualGraph(n);
    for (int i = 0; i < n; i++) {
        for (size_t j = 0; j < graph[i].size(); j++) {
            int node = graph[i][j].first;
            int capacity = graph[i][j].second;
            residualGraph[i][node] = capacity;
            residualGraph[node][i] = 0;
        }
    }

    while (true) {
        queue<int> q;
        vector<int> pred(n, -1);
        q.push(source);
        pred[source] = source;
        while (!q.empty()) {
            int v = q.front();
            q.pop();
            for (map<int,int>::iterator it = residualGraph[v].begin(); it != residualGraph[v].end(); it++) {
                if (it->second > 0 && pred[it->first] == -1) {
                    q.push(it->first);
                    pred[it->first] = v;
                }
            }
        }
        if (pred[sink] == -1) {
            break;
        }
        int v = sink;
        int additional_flow = 10000000;
        while (v != source) {
            int prev = pred[v];
            additional_flow = min(additional_flow, residualGraph[prev][v]);
            v = prev;
        }
        flow += additional_flow;

        v = sink;
        while (v != source) {
            int prev = pred[v];
            residualGraph[prev][v] -= additional_flow;
            residualGraph[v][prev] += additional_flow;
            v = prev;
        }
    }

    return flow;
}

int getNodeName(int row, int col, int inOut, int width) {
    return 1 + 2*row*width + 2*col + inOut;
}

int maximumFlow(vector<vector<bool>>& path) {

    int h = path.size();
    int w = path[0].size();
    int node = 2*w*h + 2;
    vector<vector<pair<int,int>>> graph(node);

    // Source to first row, last row to sink
    for (int i = 0; i < w; i++) {
        graph[0].push_back(make_pair(getNodeName(0, i, IN, w), 1));
        graph[getNodeName(h-1, i, OUT, w)].push_back(make_pair(2*w*h+1, 1));
    }

    // Internal
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            if (path[i][j]) {
                graph[getNodeName(i, j, IN, w)].push_back(make_pair(getNodeName(i, j, OUT, w), 1));
            }
        }
    }

    // Between rows
    for (int i = 0; i < h-1; i++) {
        for (int j = 0; j < w; j++) {
            if (j > 0) {
                graph[getNodeName(i, j, OUT, w)].push_back(make_pair(getNodeName(i, j-1, IN, w), 1));
            }
            if (j < w-1) {
                graph[getNodeName(i, j, OUT, w)].push_back(make_pair(getNodeName(i, j+1, IN, w), 1));
            }
            graph[getNodeName(i, j, OUT, w)].push_back(make_pair(getNodeName(i+1, j, IN, w), 1));
        }
    }

    return maximumFlowGraph(graph);
}

int main() {

    int T;
    cin >> T;
    for (int t = 1; t <= T; t++) {
        int h, w, b;
        cin >> w >> h >> b;
        vector<vector<bool>> path(h, vector<bool>(w, true));
        for (int i = 0; i < b; i++) {
            int x0, y0, x1, y1;
            cin >> x0 >> y0 >> x1 >> y1;
            for (int i = y0; i <= y1; i++) {
                for (int j = x0; j <= x1; j++) {
                    path[i][j] = false;
                }
            }
        }
        cout << "Case #" << t << ": " << maximumFlow(path) << endl;
    }

    return 0;
}

