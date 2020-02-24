#include <cstdio>
#include <functional>
#include <queue>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

// tests
// Kaka's matrix travel (POJ)
// https://www.acmicpc.net/problem/8992
// SRM 506 mid
// ACM-ICPC World Finals 2015 C Catering

// precondition: there is no negative cycle.
// usage:
//  MinCostFlow mcf(n);
//  for(each edges) mcf.addEdge(from, to, cost, capacity);
//  mcf.solve(source, sink); // min cost max flow
//  mcf.solve(source, sink, 0); // min cost flow
//  mcf.solve(source, sink, goal_flow); // min cost flow with total_flow >= goal_flow if possible
struct MinCostFlow
{
    typedef int cap_t;
    typedef int cost_t;

    bool iszerocap(cap_t cap) { return cap == 0; }

    struct edge {
        int target;
        cost_t cost;
        cap_t residual_capacity;
        cap_t orig_capacity;
        size_t revid;
    };

    int n;
    vector<vector<edge>> graph;
    vector<cost_t> pi;
    bool needNormalize, ranbefore;
    int lastStart;

    MinCostFlow(int n) : graph(n), n(n), pi(n, 0), needNormalize(false), ranbefore(false) {}
    void addEdge(int s, int e, cost_t cost, cap_t cap)
    {
        if (s == e) return;
        edge forward={e, cost, cap, cap, graph[e].size()};
        edge backward={s, -cost, 0, 0, graph[s].size()};
        if (cost < 0 || ranbefore) needNormalize = true;
        graph[s].emplace_back(forward);
        graph[e].emplace_back(backward);
    }
    bool normalize(int s) {
        auto infinite_cost = numeric_limits<cost_t>::max();
        vector<cost_t> dist(n, infinite_cost);
        dist[s] = 0;
        queue<int> q;
        vector<int> v(n), relax_count(n);
        v[s] = 1; q.push(s);
        while(!q.empty()) {
            int cur = q.front();
            v[cur] = 0; q.pop();
            if (++relax_count[cur] >= n) return false;
            for (const auto &e : graph[cur]) {
                if (iszerocap(e.residual_capacity)) continue;
                auto next = e.target;
                auto ncost = dist[cur] + e.cost;
                if (dist[next] > ncost) {
                    dist[next] = ncost;
                    if (v[next]) continue;
                    v[next] = 1; q.push(next);
                }
            }
        }
        for (int i = 0; i < n; i++) pi[i] = dist[i];
        return true;
    }

    pair<cost_t, cap_t> AugmentShortest(int s, int e, cap_t flow_limit) {
        auto infinite_cost = numeric_limits<cost_t>::max();
        auto infinite_flow = numeric_limits<cap_t>::max();
        typedef pair<cost_t, int> pq_t;
        priority_queue<pq_t, vector<pq_t>, greater<pq_t>> pq;
        vector<pair<cost_t, cap_t>> dist(n, make_pair(infinite_cost, 0));
        vector<int> from(n, -1), v(n);

        if (needNormalize || (ranbefore && lastStart != s))
            normalize(s);
        ranbefore = true;
        lastStart = s;

        dist[s] = pair<cost_t, cap_t>(0, infinite_flow);
        pq.emplace(dist[s].first, s);
        while(!pq.empty()) {
            auto cur = pq.top().second; pq.pop();
            if (v[cur]) continue;
            v[cur] = 1;
            if (cur == e) continue;
            for (const auto &e : graph[cur]) {
                auto next = e.target;
                if (v[next]) continue;
                if (iszerocap(e.residual_capacity)) continue;
                auto ncost = dist[cur].first + e.cost - pi[next] + pi[cur];
                auto nflow = min(dist[cur].second, e.residual_capacity);
                if (dist[next].first <= ncost) continue;
                dist[next] = make_pair(ncost, nflow);
                from[next] = e.revid;
                pq.emplace(dist[next].first, next);
            }
        }
        /** augment the shortest path **/
        auto p = e;
        auto pathcost = dist[p].first + pi[p] - pi[s];
        auto flow = dist[p].second;
        if (iszerocap(flow)|| (flow_limit <= 0 && pathcost >= 0)) return pair<cost_t, cap_t>(0, 0);
        if (flow_limit > 0) flow = min(flow, flow_limit);
        /* update potential */
        for (int i = 0; i < n; i++) {
            if (iszerocap(dist[i].second)) continue;
            pi[i] += dist[i].first;
        }
        while (from[p] != -1) {
            auto nedge = from[p];
            auto np = graph[p][nedge].target;
            auto fedge = graph[p][nedge].revid;
            graph[p][nedge].residual_capacity += flow;
            graph[np][fedge].residual_capacity -= flow;
            p = np;
        }
        return make_pair(pathcost * flow, flow);
    }

    pair<cost_t,cap_t> solve(int s, int e, cap_t flow_minimum = numeric_limits<cap_t>::max()) {
        cost_t total_cost = 0;
        cap_t total_flow = 0;
        for(;;) {
            auto res = AugmentShortest(s, e, flow_minimum - total_flow);
            if (res.second <= 0) break;
            total_cost += res.first;
            total_flow += res.second;
        }
        return make_pair(total_cost, total_flow);
    }
};

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "tc %d\n", ti);
        int n, c, m; scanf("%d%d%d", &n, &c, &m);
        vector<vector<int>> a(2);
        for(int i=0;i<m;i++){
            int p, b; scanf("%d%d", &p, &b);
            a[b-1].push_back(p);
        }
        sort(a[0].begin(), a[0].end());
        sort(a[1].begin(), a[1].end());
        int k = a[0].size();
        MinCostFlow mcf(m+2); int src = 0, dst = m+1;
        for(int i=0;i<a[0].size();i++){
            mcf.addEdge(src, i+1, 0, 1);
        }
        for(int i=0;i<a[1].size();i++){
            mcf.addEdge(k+i+1, dst, 0, 1);
        }
        for(int i=0;i<a[0].size();i++){
            for(int j=0;j<a[1].size();j++){
                if(a[0][i] != a[1][j]){
                    mcf.addEdge(i+1, k+j+1, 0, 1);
                }
                else if(a[0][i] != 1){
                    mcf.addEdge(i+1, k+j+1, 1, 1);
                }
            }
        }
        auto r = mcf.solve(src, dst);
        int promo = r.first;
        int overl = r.second;
        printf("Case #%d: %d %d\n", ti, m - overl, promo);
    }
    return 0;
}
