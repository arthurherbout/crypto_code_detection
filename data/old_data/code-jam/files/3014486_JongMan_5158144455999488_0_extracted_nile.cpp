#include<cstdio>
#include<cassert>
#include<cstring>
#include<map>
#include<set>
#include<time.h>
#include<algorithm>
#include<stack>
#include<queue>
#include<utility>
#include<cmath>
#include<iostream>
#include<string>
#include<vector>
using namespace std;

struct NetworkFlow
{
    struct Edge
    {
        int target, cap, flow; Edge* reverse;           
        Edge(int t, int c) : target(t), cap(c), flow(0), reverse(NULL) {}
        int res() const { return cap - flow; }
        void push(int c) { flow += c; reverse->flow = -flow; }
    };
    int totalFlow, V;
    vector<vector<Edge*> > adj;
    NetworkFlow(int V): totalFlow(0), V(V), adj(V) {}
    ~NetworkFlow() { for(int i = 0; i < V; ++i) for(int j = 0; j < adj[i].size(); ++j) delete adj[i][j]; }
    void addEdge(int a, int b, int a2b, int b2a = 0) 
    {
        Edge *ab = new Edge(b, a2b), *ba = new Edge(a, b2a);
        ab->reverse = ba; ba->reverse = ab;
        adj[a].push_back(ab);
        adj[b].push_back(ba);           
    }
    int pushFlow(int source, int sink)
    {
        vector<Edge*> pred(V, (Edge*)NULL);
        queue<int> q;
        q.push(source); 
        while(!q.empty() && pred[sink] == NULL)
        {
            int u = q.front(); q.pop();
            for(int i = 0; i < adj[u].size(); ++i)                  
            {
                Edge* e = adj[u][i];
                int v = e->target;
                if(e->res() > 0 && pred[v] == NULL && v != source) { pred[v] = e->reverse; q.push(v); }                 
            }
        }
        if(pred[sink] == NULL) return 0;            
        int h, amt = 2147483647;
        h = sink; while(source != h) { amt = min(amt, pred[h]->reverse->res()); h = pred[h]->target; }
        h = sink; while(source != h) { pred[h]->reverse->push(amt); h = pred[h]->target; }
        totalFlow += amt;
        return amt;
    }
    int go(int source = 0, int sink = 1)
    {
        int ret = 0, f;
        while((f = pushFlow(source, sink)))
            ret += f;
        return ret;
    }
};

int no[501][101];

int incoming(int y, int x) {
  int idx = no[y][x];
  assert(idx != -1);
  return 2 + 2 * idx;
}

int outgoing(int y, int x) {
  int idx = no[y][x];
  assert(idx != -1);
  return 2 + 2 * idx + 1;
}

int main() {
  int cases;
  cin >> cases;
  for(int cc = 0; cc < cases; ++cc) {
    memset(no, 0, sizeof(no)); 
    int w, h, b;
    cin >> w >> h >> b;
    for(int i = 0; i < b; ++i) {
      int x0, x1, y0, y1;
      cin >> x0 >> y0 >> x1 >> y1;
      for(int y = y0; y < y1+1; ++y) 
        for(int x = x0; x < x1+1; ++x) 
          no[y][x] = -1;
    }

    int cells = 0;
    for(int i = 0; i < h; ++i) 
      for(int j = 0; j < w; ++j) 
        if(no[i][j] != -1) {
          no[i][j] = cells++;
        }

    NetworkFlow flow(2 + cells * 2);
    for(int i = 0; i < h; ++i) 
      for(int j = 0; j < w; ++j) {
        if(no[i][j] == -1) continue;

        flow.addEdge(incoming(i, j), outgoing(i, j), 1);
        if(i == 0) flow.addEdge(outgoing(i, j), 1, 1);
        if(i == h-1) flow.addEdge(0, incoming(i, j), 1);

        if(i + 1 < h && no[i+1][j] != -1) {
          flow.addEdge(outgoing(i, j), incoming(i+1, j), 1);
          flow.addEdge(outgoing(i+1, j), incoming(i, j), 1);
        }
        if(j + 1 < w && no[i][j+1] != -1) {
          flow.addEdge(outgoing(i, j), incoming(i, j+1), 1);
          flow.addEdge(outgoing(i, j+1), incoming(i, j), 1);
        }
      }

    while(flow.pushFlow(0, 1));

    printf("Case #%d: %d\n", cc+1, flow.totalFlow);
  }
}

