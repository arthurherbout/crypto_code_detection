#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;

#define X first
#define Y second

typedef long long ll;
typedef long double ld;
typedef pair<ll, ll> P;
typedef vector<bool> Vb;
typedef vector<Vb> Mb;
typedef vector<char> Vc;
typedef vector<Vc> Mc;
typedef vector<int> Vi;
typedef vector<Vi> Mi;
typedef vector<ll> Vll;
typedef vector<Vll> Mll;
typedef vector<P> Vp;
typedef vector<Vp> Mp;
typedef vector<string> Vs;
typedef vector<Vs> Ms;

typedef set<int> SET;
typedef SET::iterator Sit;
typedef map<int, int> MAP;
typedef MAP::iterator Mit;
typedef stringstream SS;

template <class Ta, class Tb> inline Tb cast(Ta a) { SS ss; ss << a; Tb b; ss >> b; return b; };

const double EPS = 1e-9;
const ll INF = 1000000000000000000LL;
const int diri[8] = { -1, 0, 1, 0, -1, 1, 1, -1 };
const int dirj[8] = { 0, 1, 0, -1, 1, 1, -1, -1 };

struct Edge {
  int a, b;
  ll d1, d2;
};

int N, M, K;
vector<Edge> edges;

Mi net;
Mll netd1, netd2;

Mi netr;
Mll netrd1, netrd2;

Vi prop;

void dijkstra(int a, const Mi& g, const Mll& gd, Vll& d) {
  int n = g.size();
  d = Vll(n, INF);
  d[a] = 0;
  priority_queue<P> q;
  q.push(P(0, a));
  while (not q.empty()) {
    int nod = q.top().second;
    ll dis = -q.top().first;
    q.pop();
    if (dis != d[nod]) continue;
    for (int i = 0; i < int(g[nod].size()); ++i) {
      int nod2 = g[nod][i];
      ll dis2 = dis + gd[nod][i];
      if (dis2 < d[nod2]) {
        d[nod2] = dis2;
        q.push(P(-dis2, nod2));
      }
    }
  }
}

int main() {
  int tcas;
  cin >> tcas;
  for (int cas = 1; cas <= tcas; ++cas) {
    cin >> N >> M >> K;
    edges = vector<Edge>(M);
    net = netr = Mi(N);
    netd1 = netd2 = netrd1 = netrd2 = Mll(N);
    for (int i = 0; i < M; ++i) {
      cin >> edges[i].a >> edges[i].b >> edges[i].d1 >> edges[i].d2;
      --edges[i].a;
      --edges[i].b;
      net[edges[i].a].push_back(edges[i].b);
      netd1[edges[i].a].push_back(edges[i].d1);
      netd2[edges[i].a].push_back(edges[i].d2);
      netr[edges[i].b].push_back(edges[i].a);
      netrd1[edges[i].b].push_back(edges[i].d1);
      netrd2[edges[i].b].push_back(edges[i].d2);
    }
    prop = Vi(K);
    for (int i = 0; i < K; ++i) {
      cin >> prop[i];
      --prop[i];
    }
    
    Mll d1(N), d2(N);
    for (int i = 0; i < N; ++i)
      dijkstra(i, net, netd1, d1[i]);
    for (int i = 0; i < N; ++i)
      dijkstra(i, net, netd2, d2[i]);
    
    int res = -1;
    
    Vll sum(K);
    sum[0] = prop[0];
    for (int i = 1; i < K; ++i)
      sum[i] = sum[i - 1] + prop[i];
    
    ll acum = 0;
    for (int i = 0; i < K; ++i) {
      int e = prop[i];
      acum += edges[e].d1;
      
      for (int j = 0; j <= i; ++j) {
        ll mc = sum[i] - (j == 0 ? 0 : sum[j - 1]);
        if (d2[edges[prop[j]].a][edges[prop[i]].b] < mc) {
          res = prop[i];
          break;
        }
      }
      if (res != -1) break;
      
      if (acum + d1[edges[e].b][1] > d2[0][1]) {
        res = prop[i];
        break;
      }
    }
    
    cout << "Case #" << cas << ": ";
    if (res < 0) cout << "Looks Good To Me" << endl;
    else cout << res + 1 << endl;
  }
}
