#include <bits/stdc++.h>
using namespace std;
#define sim template < class c
#define ris return * this
#define dor > debug & operator <<
#define eni(x) sim > typename \
  enable_if<sizeof dud<c>(0) x 1, debug&>::type operator<<(c i) {
sim > struct rge { c b, e; };
sim > rge<c> range(c i, c j) { return rge<c>{i, j}; }
sim > auto dud(c* x) -> decltype(cerr << *x, 0);
sim > char dud(...);
struct debug {
#ifdef LOCAL
~debug() { cerr << endl; }
eni(!=) cerr << boolalpha << i; ris; }
eni(==) ris << range(begin(i), end(i)); }
sim, class b dor(pair< b, c > d) {
  ris << "(" << d.first << ", " << d.second << ")";
}
sim dor(rge<c> d) {
  *this << "[";
  for (auto it = d.b; it != d.e; ++it)
    *this << ", " + 2 * (it == d.b) << *it;
  ris << "]";
}
#else
sim dor(const c&) { ris; }
#endif
};
#define imie(...) " [" << #__VA_ARGS__ ": " << (__VA_ARGS__) << "] "










// flow implementation from Warsaw Uni library

/* DINIC O(n^2*m) lub O(MAXFLOW*(n+m)).                                       */
/* LINIOWA PAMIĘĆ - główny powód stosowania, dla grafów z krawędziami         */
/* jednostykowymi działa w O(m*sqrt(n))                                       */
/* Szybkie, można ryzykować, w razie czego shufflowac krawędzie.              */
/* Używamy funkcji add_edge().                                                */
/* Używamy funkcji dinic(), która modyfikuje krawędzie i zwraca wartość       */
/* maksymalnego przepływu.                                                    */
/* Na krawędziach zostaje flow, do odtwarzania wyniku.                        */

typedef long long ll;

struct flow {
  struct edge {  // Można dodawać atrybuty w celu identyfikacji krawędzi.
    int cel = 0;
    ll lim = 0;
    ll flow = 0;
    edge* odw;
  };

  //liczba wierzcholkow, numer zrodla i numer ujscia, numerowane od 1
  int n, zr, uj;

  vector<vector<edge*>> graf;
  vector<vector<edge*>> dgraf;
  vector<int> odl;

  flow(){}

  flow(int vertex_limit) {
    graf.resize(vertex_limit + 1);
    dgraf.resize(vertex_limit + 1);
    odl.resize(vertex_limit + 1, 0);
  }

  // Dodaje krawędź od @v do @u o przepustowosci @lim.
  void add_edge(int v, int u, ll lim, bool bi_dir) {
	 // debug() << imie(v) << imie(u);
    edge* wte = new edge();
    edge* wtamte = new edge();
    wte->odw = wtamte;
    wtamte->odw = wte;
    wte->lim = lim;
    if (bi_dir) wtamte->lim = lim;
    wte->cel = u;
    wtamte->cel = v;
    graf[v].push_back(wte);
    graf[u].push_back(wtamte);
  }

  // progarm używa się wewnętrznie, nie tykać
  void clear() {
    for (int i = 1; i <= n; i++) {
      odl[i] = 0;
      dgraf[i].clear();
    }
  }

  // progarm używa się wewnętrznie, nie tykać
  void bfs() {
    queue<int> kol;
    int u;
    kol.push(zr);
    odl[zr] = 1;
    while (!kol.empty()) {
      u = kol.front();
      kol.pop();
      for (int i = 0; i < (int) graf[u].size(); i++) {
        if (!odl[graf[u][i]->cel] && graf[u][i]->lim) {
          odl[graf[u][i]->cel] = odl[u] + 1;
          kol.push(graf[u][i]->cel);
        }
        if (odl[graf[u][i]->cel] == odl[u] + 1)
          dgraf[u].push_back(graf[u][i]);
      }
    }
  }

  // progarm używa się wewnętrznie, nie tykać
  ll dfs(int v, ll mini) {
    if (v==uj) return mini;
    for (int i = 0; i < (int) dgraf[v].size(); i++) {
      if (dgraf[v][i]->lim) {
        ll ret = dfs(dgraf[v][i]->cel, min(mini, dgraf[v][i]->lim));
        if (!ret) {
          swap(dgraf[v][i], dgraf[v].back());
          dgraf[v].pop_back();
          i--;
          continue;
        }
        dgraf[v][i]->lim -= ret;
        dgraf[v][i]->odw->lim += ret;
        dgraf[v][i]->flow += ret;
        ll pom = min(dgraf[v][i]->flow, dgraf[v][i]->odw->flow);
        dgraf[v][i]->flow -= pom;
        dgraf[v][i]->odw->flow -= pom;
        return ret;
      } else {
        swap(dgraf[v][i], dgraf[v].back());
        dgraf[v].pop_back();
        i--;
      }
    }
    return 0;
  }

  ll dinic() {
    ll ret = 0;
    while (true) {
      clear();
      bfs();
      ll bier1 = 0;
      while (true) {
        // stała większa niż jakakolwiek przepustowość
        ll bier2 = dfs(zr, 1000 * 1000 * 1000);
        if (!bier2) break;
        bier1 += bier2;
      }
      if (!bier1) break;
      ret += bier1;
    }
    return ret;
  }
};
























const int inf = 1e9 + 5;
const int nax = 505;

// flow implementation from Warsaw Uni library

#define pb push_back
#define FOR(i,a,b) for(int i = (a); i <= (b); ++i)
#define REP(i,n) FOR(i,0,(n)-1)
#define mp make_pair

//c - capacity, v - value
//wierzcholki sa od 0 do n, wszystko na intach 
//wiec wtedy musisz ogarnac odlegloci i takie tam
 
  struct Edge{
    int w,c,v,rev;
    Edge(int _w, int _c, int _v, int _rev) :
      w(_w), c(_c), v(_v), rev(_rev)
    {}
    void print() {
		debug() << imie(w) << imie(c);
	}
  };
  
  int odl[nax], pot[nax], pop[nax], pop_kraw[nax];
  int q[nax], qbeg, qend;
  vector<Edge> v[nax];
  bool bylo[nax];
  queue<int> kolej;
  //priority_queue<pair<int,int> > kolej;
  
  void init(int n) {
    FOR(i,0,n) v[i].clear();
  }
  
  void AddEdge(int a, int b, int cap, int cost) {
    v[a].pb(Edge(b,cap,cost,int(v[b].size()) + (a == b)));
    v[b].pb(Edge(a,0,-cost,int(v[a].size()-1)));
  }
  
  pair<int,int> MinCostMaxFlow(int s, int t, int n) {
    int flow = 0, cost = 0;
    //FOR(i,0,n) pot[i] = 0; 
    //potencjaly zerowe dzialaja dla samych nieujemnych kosztow
    //jak sa ujemne krawedzie to bellman
    
    while (true) {
      FOR(i,0,n) {
        odl[i] = inf;
        bylo[i] = false;
      }
      bylo[s] = true;
      odl[s] = 0;
      kolej.push(s);
      
      //djikstra, mozna napisac na kolejce
      while(!kolej.empty()) {
        int x = kolej.front();
        kolej.pop();
        bylo[x] = false;

        //if (bylo[x])
        //  continue;
        
        /*int najm = inf;
        FOR(i,0,n) if (!bylo[i] && najm > odl[i]) {
          x = i; najm = odl[i];
        }*/
        
        //bylo[x] = true;
        int dl = v[x].size();
        REP(i,dl) if (v[x][i].c > 0 && 
          odl[v[x][i].w] > 
            odl[x] + pot[x] - pot[v[x][i].w] + v[x][i].v) {
          odl[v[x][i].w] = 
            odl[x] + pot[x] - pot[v[x][i].w] + v[x][i].v;
          if (!bylo[v[x][i].w]) {
            kolej.push(v[x][i].w);
            bylo[v[x][i].w] = true;
          }
          //kolej.push(mp(-odl[v[x][i].w], v[x][i].w));
          pop[v[x][i].w] = x; pop_kraw[v[x][i].w] = i; 
        } 
      }
      
      if (odl[t] == inf)
        break;
      
      //FOR(i,0,n) pot[i] += odl[i];
      
      int x = t;
      int cap = inf;
      while (x != s) {
        cap = min(cap, v[pop[x]][pop_kraw[x]].c);
        x = pop[x];
      }
      
      flow += cap;
      x = t;
      while (x != s) {
		  //debug() << imie(
        cost += v[pop[x]][pop_kraw[x]].v*cap;
        v[pop[x]][pop_kraw[x]].c -= cap;
        v[x][v[pop[x]][pop_kraw[x]].rev].c += cap;
        x = pop[x];
      }
    }
    
   /* for(int i = 1; i <= n; ++i)
		for(Edge e : v[i]) {
			
			debug() << "edge from " << i << ":";
			e.print();
			debug();
		}*/
    
    return mp(flow, cost);
  }

pair<int,int> getFlowCost(vector<vector<int>> edges, int cnt_sol, int cnt_tow, vector<pair<int,int>> & rozw) {
	init(cnt_sol + cnt_tow + 3);
	for(int i = 1; i <= cnt_sol; ++i)
		AddEdge(1, i + 1, 1, 0);
	for(vector<int> e : edges) {
		assert((int) e.size() == 3);
		AddEdge(2 + e[0], 2 + cnt_sol + e[1], 1, e[2]);
	}
	for(int i = 1; i <= cnt_tow; ++i)
		 AddEdge(1 + cnt_sol + i, cnt_sol + cnt_tow + 2, 1, 0);
	pair<int,int> ret =
			MinCostMaxFlow(1, cnt_sol + cnt_tow + 2, cnt_sol + cnt_tow + 3);
	
	for(int i = 1; i <= cnt_sol; ++i)
		for(Edge e : v[i+1]) {
			if(e.w > cnt_sol + 1 && e.c == 0)
				rozw.push_back(make_pair(i, e.w - cnt_sol - 1));
				//printf("%d -> %d\n", i, e.w - cnt_sol - 1);
		}
	return ret;
}









const int dx[4] = {-1,1,0,0};
const int dy[4] = {0,0,-1,1};
int h, w, M;
char in[nax][nax];
int id[nax][nax];
bool vis[nax][nax];
vector<int> inv[nax][nax];

bool inside(int row, int col) {
	return 0 <= row && row < h && 0 <= col && col < w;
}

void clear(int n) {
	for(int i = 0; i <= n; ++i)
		for(int j = 0; j <= n; ++j) {
			id[i][j] = 0;
			inv[i][j].clear();
		}
}

int getFlow(vector<vector<int>> edges, int cnt_sol, int cnt_tow) {
	flow f(cnt_sol + cnt_tow + 2);
	for(int i = 1; i <= cnt_sol; ++i)
		f.add_edge(1, i + 1, 1, false);
	for(vector<int> e : edges) {
		assert((int) e.size() == 3);
		f.add_edge(2 + e[0], 2 + cnt_sol + e[1], 1, false);
	}
	for(int i = 1; i <= cnt_tow; ++i)
		f.add_edge(1 + cnt_sol + i, cnt_sol + cnt_tow + 2, 1, false);
	f.zr = 1;
	f.uj = f.n = cnt_sol + cnt_tow + 2;
	return f.dinic();
}

void test_case() {
	scanf("%d%d%d", &w, &h, &M);
	vector<pair<int,int>> soldiers, towers;
	for(int row = 0; row < h; ++row) {
		scanf("%s", in[row]);
		for(int col = 0; col < w; ++col) {
			if(in[row][col] == 'S') {
				id[row][col] = soldiers.size();
				soldiers.push_back(make_pair(row, col));
			}
			else if(in[row][col] == 'T') {
				id[row][col] = towers.size();
				towers.push_back(make_pair(row, col));
			}
			else id[row][col] = -1;
		}
	}
	for(int which = 0; which < (int) towers.size(); ++which)
		for(int dir = 0; dir < 4; ++dir) {
			int row = towers[which].first + dx[dir];
			int col = towers[which].second + dy[dir];
			while(inside(row, col) && in[row][col] != '#') {
				inv[row][col].push_back(which);
				row += dx[dir];
				col += dy[dir];
			}
		}
	
	vector<vector<int>> edges;
	vector<vector<bool>> done(soldiers.size(), vector<bool>(towers.size(), false));
	
	for(int who = 0; who < (int) soldiers.size(); ++who) {
		// bfs
		vector<pair<int,int>> now, then;
		now.push_back(soldiers[who]);
		vis[soldiers[who].first][soldiers[who].second] = true;
		for(int already = 0; already <= M; ++already) {
			for(pair<int,int> p : now) {
				const int row = p.first, col = p.second;
				assert(inside(row, col));
				for(int which : inv[row][col])
					if(!done[who][which]) {
						done[who][which] = true;
						edges.push_back(vector<int>{who, which, already});
					}
				for(int dir = 0; dir < 4; ++dir) {
					const int r2 = row + dx[dir];
					const int c2 = col + dy[dir];
					if(!inside(r2, c2) || vis[r2][c2] || in[r2][c2] == '#')
						continue;
					then.push_back({r2, c2});
					vis[r2][c2] = true;
				}
			}
			now.clear();
			swap(now, then);
		}
		for(int row = 0; row < h; ++row)
			for(int col = 0; col < w; ++col)
				vis[row][col] = false;
	}
	
	sort(edges.begin(), edges.end(), [&] (const vector<int> & a, const vector<int> & b) {
		return a[2] > b[2];
	});
	
	debug() << imie(edges) << "\n";
	
	auto f_flow = [&] (vector<vector<int>> ee) {
		return getFlow(ee, soldiers.size(), towers.size());
	};
	
	//auto f_cost = [&] (vector<vector<int>> ee) {
	vector<pair<int,int>> rozw;
	pair<int,int> parka = getFlowCost(edges, soldiers.size(), towers.size(), rozw);
	
	assert(parka.first == f_flow(edges));
	
	random_shuffle(rozw.begin(), rozw.end());
	
	debug() << imie(rozw);
	
	int answer = parka.first;
	printf("%d\n", answer);
	
	assert(answer == (int) rozw.size());
	
	for(int repeat = 0; repeat < answer; ++repeat) {
		random_shuffle(rozw.begin(), rozw.end());
		for(int i = 0; i < (int) rozw.size(); ++i) {
			int who = rozw[i].first - 1;
			int spec_which = rozw[i].second - 1;
			vector<pair<int,int>> now, then;
			now.push_back(soldiers[who]);
			vis[soldiers[who].first][soldiers[who].second] = true;
			bool can = false, bad = false;
			for(int already = 0; already <= M && !bad && !can; ++already) {
				for(pair<int,int> p : now) {
					const int row = p.first, col = p.second;
					assert(inside(row, col));
					for(int which : inv[row][col]) {
						if(spec_which == which) {
							can = true;
							break;
						}
					}
					if(!can && !inv[row][col].empty()) {
						bad = true;
						break;
					}
					for(int dir = 0; dir < 4; ++dir) {
						const int r2 = row + dx[dir];
						const int c2 = col + dy[dir];
						if(!inside(r2, c2) || vis[r2][c2] || in[r2][c2] == '#')
							continue;
						then.push_back({r2, c2});
						vis[r2][c2] = true;
					}
				}
				now.clear();
				swap(now, then);
			}
			for(int row = 0; row < h; ++row)
				for(int col = 0; col < w; ++col)
					vis[row][col] = false;
			
			assert(!(can && bad));
			if(!can) continue;
			printf("%d %d\n", who + 1, spec_which + 1);
			int which = spec_which;
			for(int dir = 0; dir < 4; ++dir) {
				int row = towers[which].first + dx[dir];
				int col = towers[which].second + dy[dir];
				while(inside(row, col) && in[row][col] != '#') {
					vector<int> & vec = inv[row][col];
					for(int j = 0; j < (int) vec.size(); ++j)
						if(vec[j] == which) {
							swap(vec[j], vec.back());
							vec.pop_back();
							break;
						}
					//for(int j = 0; j < (int) inv[row][
					//inv[row][col].push_back(which);
					row += dx[dir];
					col += dy[dir];
				}
			}
			swap(rozw[i], rozw.back());
			rozw.pop_back();
			break;
		}
	}
	
	//};
	
	//debug() << imie(f_flow(edges));
	//debug() << imie(f_cost(edges)) << "\n";
	/*printf("%d\n", answer);
	
	for(int row = 0; row < h; ++row) {
		for(int col = 0; col < w; ++col) printf("%d", id[row][col] + 1);
		puts("");
	}
	
	while(!edges.empty()) {
		vector<int> maybe = edges.back();
		edges.pop_back();
		vector<vector<int>> nowy;
		for(vector<int> e : edges)
			if(e[0] != maybe[0] && e[1] != maybe[1])
				nowy.push_back(e);
		if(f(nowy) == answer - 1) {
			--answer;
			edges = nowy;
			printf("%d %d  cost = %d\n", maybe[0] + 1, maybe[1] + 1, maybe[2]);
		}
	}*/
	
	clear(max(w, h));
}

int main(int argc, char * argv[]) {
	int T;
	scanf("%d", &T);
	for(int nr = 1; nr <= T; ++nr) {
		printf("Case #%d: ", nr);
		test_case();
	}
}
