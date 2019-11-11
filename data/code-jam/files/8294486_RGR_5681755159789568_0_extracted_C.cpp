#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using vl = vector<ll>;
using vvl=vector<vl>;
using vb=vector<bool>;
using vs=vector<string>;
using pll=pair<ll,ll>;
const ll oo = 0x3f3f3f3f3f3f3f3fLL;
const double eps = 1e-9;
#define sz(c) ll((c).size())
#define all(c) begin(c),end(c)
#define mp make_pair
#define mt make_tuple
#define pb push_back
#define eb emplace_back
#define xx first
#define yy second
#define FR(i,a,b) for(ll i = (a); i < (b); i++)
#define FRD(i,a,b) for(ll i = ll(b)-1;i>=(a);i--)
#define FOR(i,a,b) for (ll i = (a); i < (b); i++)
#define FORD(i,a,b) for (ll i = ll(b)-1; i >= (a); i--)
#define FORIT(i,c) for (__typeof__((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define TR(X) ({if(1) cerr << "TR: " << (#X) << " = " << (X) << endl; })

//#define N 100000
//#define _MAX LLONG_MAX
//
//typedef struct Edge {
//	ll to, len;
//} edge;
//
//typedef struct Node {
//	ll dist, prev;
//	list<Edge> out;
//} node;

typedef struct Horse {
	ll stamina, speed;
} horse;

typedef struct Node {
	//ll time, remain;
	ll remain = 0;
	double time = DBL_MAX;
} node;

//ll actualN;
//Node nodes[N];
//
//void dijkstra(ll start) {
//	FOR(i, 0LL, actualN) {
//		nodes[i].dist = _MAX;
//		nodes[i].prev = -1LL;
//	}
//	nodes[start].dist = 0LL;
//	set<pll> pq;
//	pq.insert(pll(0LL, start));
//	while (!pq.empty()) {
//		ll cur = pq.begin()->second;
//		pq.erase(pq.begin());
//		FORIT(i, nodes[cur].out) {
//			if (nodes[cur].dist + i->len >= nodes[i->to].dist) continue;
//			if (nodes[i->to].dist < _MAX) pq.erase(pq.find(pll(nodes[i->to].dist, i->to)));
//			nodes[i->to].dist = nodes[cur].dist + i->len;
//			nodes[i->to].prev = cur;
//			pq.insert(pll(nodes[i->to].dist, i->to));
//		}
//	}
//}

int main() {
	ios_base::sync_with_stdio(false);
	int tc;
	cin >> tc;
	FR(asdf,0,tc) {
		ll n, q;
		cin >> n >> q;
		vector<horse> horses(n);
		FR(a,0,n) {
			cin >> horses[a].stamina >> horses[a].speed;
		}
		vl adj(n);
		FR(a,0,n) {
			FR(b,0,n) {
				ll dist;
				cin >> dist;
				if(dist != -1LL) {
					adj[a] = dist;
				}
			}
		}
		ll tmp;
		cin >> tmp >> tmp;
		vector<vector<node>> nodes(n);
		FR(a,0,n) {
			nodes[a].resize(n);
		}
		node& start = nodes[0][1];
		start.remain = horses[0].stamina - adj[0];
		start.time = ((double)adj[0]) / ((double)horses[0].speed);
		FR(b,1,n-1) {
			FR(a,0,b+1) {
				//a
				if(nodes[a][b].remain >= adj[b]) {
					nodes[a][b+1].remain = nodes[a][b].remain - adj[b];
					nodes[a][b+1].time = nodes[a][b].time + ((double)adj[b]) / ((double)horses[a].speed);
				}
				//b
				double swn = nodes[a][b].time + ((double)adj[b]) / ((double)horses[b].speed);
				if(swn < nodes[b][b+1].time) {
					nodes[b][b+1].remain = horses[b].stamina - adj[b];
					nodes[b][b+1].time = swn;
				}
			}
		}
		double best = DBL_MAX;
		FR(a,0,n) {
			best = min(best, nodes[a][n-1].time);
		}
		cout.precision(12);
		cout << "Case #" << (asdf+1) << ": " << fixed << best << '\n';
	}
}
