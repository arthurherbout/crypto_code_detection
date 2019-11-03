#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cassert>
#include <limits>
#include <queue>
#include <algorithm>
#include <set>
#include <math.h>
#include <sstream>

using namespace std;

struct Node;

struct Edge {
    Edge() : capacity(0), flow(0), to(0) {}
    Edge(Node* To, int Capacity) : capacity(Capacity), flow(0), to(To) {}
    
    int capacity;
    int flow;
    Node* to;
};

Edge INVALID;

struct Node {
    Node() : predecessor(0) {}
    Node(const std::string& name) : predecessor(0), Name(name) {}
    
    void connect(Node* to, int capacity) {
        // Basically want to add this->to with capacity and to->this with -capacity.
        // But ... if user ever explicitly adds an edge matching one of our auto-added
        // -capacity edges, we have their edge override that.
        bool foundFrom = false;
        for (size_t i = 0; i != edges.size(); ++i) {
            if (edges[i].to == to) {
                edges[i].capacity = capacity;
                foundFrom = true;
                break;
            }
        }
        if (!foundFrom) {
            edges.push_back(Edge(to, capacity));
        }

        bool foundTo = false;
        for (size_t i = 0; i != to->edges.size(); ++i) {
            if (to->edges[i].to == this) {
                foundTo = true;
                break;
            }
        }
        if (!foundTo) {
            to->edges.push_back(Edge(this, -capacity));
        }
    }
    Edge& findEdge(Node* to) {
        for (size_t i = 0; i != edges.size(); ++i) {
            if (edges[i].to == to) {
                return edges[i];
            }
        }
        return INVALID;
    }
    vector<Edge> edges;

    Node* predecessor; // used in augmenting-path
    std::string Name;
};

void printWholeGraph(Node* src) {
    set<Node*> visited;
    queue<Node*> Q;
    Q.push(src);
    visited.insert(src);
    
    while (!Q.empty()) {
        Node* cur = Q.front();
        Q.pop();

        const vector<Edge>& edges = cur->edges;
        for (size_t i = 0; i != edges.size(); ++i) {
            Node* to = edges[i].to;
            if (edges[i].capacity > 0) {
                cout << cur->Name << " -> " << to->Name << " cap " << edges[i].capacity << endl;
            }
            if (visited.find(to) == visited.end()) {
                Q.push(to);
                visited.insert(to);
            }
        }
    }
}

void retrace(Node* end, vector<Node*>& path) {
    Node* cur = end;
    while (cur) {
        path.push_back(cur);
        cur = cur->predecessor;
    }

    reverse(path.begin(), path.end());
}

bool findAugmenting(Node* src, Node* sink, vector<Node*>& path) {
    path.clear();
    
    set<Node*> visited;
    queue<Node*> Q;
    Q.push(src);
    visited.insert(src);
    src->predecessor = 0;
    
    while (!Q.empty()) {
        Node* cur = Q.front();
        Q.pop();

        const vector<Edge>& edges = cur->edges;
        for (size_t i = 0; i != edges.size(); ++i) {
            if (edges[i].flow < edges[i].capacity) {
                Node* to = edges[i].to;
                if (visited.find(to) == visited.end()) {
                    Q.push(to);
                    to->predecessor = cur;
                    visited.insert(to);
                    if (to == sink) {
                        retrace(sink, path);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

int addMaxFlow(const vector<Node*>& nodes) {
    int added = numeric_limits<int>::max();
    
    // pass 1, find how much to add
    Node* prev = nodes.front();
    for (size_t i = 1; i != nodes.size(); ++i) {
        Edge& edge = prev->findEdge(nodes[i]);
        int spare = edge.capacity - edge.flow;
        added = min(spare, added);
        prev = edge.to;
    }

    // pass 2, add it
    prev = nodes.front();
    for (size_t i = 1; i != nodes.size(); ++i) {
        Edge& edge = prev->findEdge(nodes[i]);
        edge.flow += added;

        Edge& reverse = nodes[i]->findEdge(prev);
        reverse.flow -= added;

        prev = edge.to;
    }
    return added;
}

int fordFulkerson(Node* src, Node* sink) {
    int flow = 0;
    vector<Node*> path;
    while (findAugmenting(src, sink, path)) {
        int oldflow = flow;
        flow += addMaxFlow(path);
        if (oldflow == flow) abort();
    }
    
    return flow;
}

struct Building {
    int x0, y0, x1, y1;
};

int DX[4] = { -1, 1, 0, 0 };
int DY[4] = { 0, 0, -1, 1 };

int maxFlow(int W, int H, const vector<Building> buildings) {
    vector<Node> In(W*H);
    vector<Node> Out(W*H);
    Node src("src");
    Node sink("sink");

    for (int x = 0; x != W; ++x) {
        for (int y = 0; y != H; ++y) {
            ostringstream ss;
            ss << "[" << x << "," << y << "]";
            In[x + y*W].Name = "In" + ss.str();
            Out[x + y*W].Name = "Out" + ss.str();
        }
    }

    vector<bool> free(W*H, true);
    for (int i = 0; i != buildings.size(); ++i) {
        const Building& B = buildings[i];
        for (int x = B.x0; x <= B.x1; ++x) {
            for (int y = B.y0; y <= B.y1; ++y) {
                free[x + y*W] = false;
            }
        }
    }

    for (int x = 0; x != W; ++x) {
        src.connect(&In[x], 1);
        Out[x + (H-1)*W].connect(&sink, 1);
    }

    for (int x = 0; x != W; ++x) {
        for (int y = 0; y != H; ++y) {
            int idx = x + y*W;
            if (free[idx]) {
                In[idx].connect(&Out[idx], 1);
            }

            for (int dir = 0; dir != 4; ++dir) {
                int x2 = x + DX[dir];
                int y2 = y + DY[dir];
                if (x2 >= 0 && x2 < W && y2 >= 0 && y2 < H) {
                    int idx2 = x2 + y2*W;
                    Out[idx].connect(&In[idx2], 1);
                }
            }
        }
    }

    // printWholeGraph(&src);
    return fordFulkerson(&src, &sink);
}

int main() {
    // ifstream in("sample.in");
    istream& in = cin;

    int C;
    in >> C;
    for (int c = 0; c != C; ++c) {
        int W, H, B;
        in >> W >> H >> B;
        vector<Building> buildings(B);
        for (int b = 0; b != B; ++b) {
            in >> buildings[b].x0 >> buildings[b].y0 >> buildings[b].x1 >> buildings[b].y1;
        }
        cout << "Case #" << (c+1) << ": " << maxFlow(W, H, buildings) << endl;
    }
    return 0;
}
