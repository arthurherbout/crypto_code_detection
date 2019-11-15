#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>
#include <queue>
#include <stack>
#include <set>
#include <map>
#include <algorithm>
#include <functional>
#include <utility>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <cstdio>

using namespace std;

#define REP(i,n) for((i)=0;(i)<(int)(n);(i)++)
#define snuke(c,itr) for(__typeof((c).begin()) itr=(c).begin();itr!=(c).end();itr++)

#define INF (1<<29)
#define _abs(x) ((x)>0?(x):-(x))

#define MAXV 2010

int parent[MAXV],rank[MAXV];

void init(int n){
	int i;
	REP(i,n) {parent[i] = i; rank[i] = 1;}
}

int root(int x){
	if(parent[x] != x) parent[x] = root(parent[x]);
	return parent[x];
}

void connect(int x, int y){
	int rx=root(x),ry=root(y);
	if(rx == ry) return;
	if(rank[rx] > rank[ry]) {parent[ry] = rx; rank[rx] += rank[ry];}
	if(rank[rx] <= rank[ry]) {parent[rx] = ry; rank[ry] += rank[rx];}
}

int C;
int from[2010], to[2010], start[2010], duration[2010];
vector <int> out_edge[2010], in_edge[2010];
int switch_cost[2010];
int next[2010];

int get_cost(int e1, int e2){
	int x = start[e1] + duration[e1];
	int y = start[e2];
	return (y % 24 - x % 24 + 24) % 24;
}

bool used[2010];
vector <vector <int> > cycles;

int get_mst(void){
	int i,j,k,l;
	
	cycles.clear();
	REP(i,2*C) used[i] = false;
	
	REP(i,2*C) if(!used[i]){
		int e = i;
		vector <int> cycle;
		
		while(1){
			if(used[e]) break;
			used[e] = true;
			cycle.push_back(from[e]);
			e = next[e];
		}
		
		cycles.push_back(cycle);
	}
	
	int N = cycles.size();
	init(N);
	
	int ans = 0;
	
	vector <pair <int, pair <int, int> > > edges;
	
	REP(i,N) REP(j,i) REP(k,cycles[i].size()) REP(l,cycles[j].size()) if(cycles[i][k] == cycles[j][l]){
		int x = cycles[i][k];
		edges.push_back(make_pair(switch_cost[x], make_pair(i, j)));
	}
	sort(edges.begin(), edges.end());
	
	REP(i,edges.size()){
		int c = edges[i].first;
		int p = edges[i].second.first;
		int q = edges[i].second.second;
		
		if(root(p) != root(q)){
			connect(p, q);
			ans += c;
		}
	}
	
	return ans;
}

void main2(void){
	int i,j;
	
	cin >> C;
	REP(i,2*C){
		cin >> to[i] >> start[i] >> duration[i];
		to[i]--;
	}
	REP(i,2*C) from[i] = i / 2;
	
	REP(i,C){
		out_edge[i].clear();
		in_edge[i].clear();
	}
	
	REP(i,2*C){
		out_edge[from[i]].push_back(i);
		in_edge[to[i]].push_back(i);
	}
	
	int extra = 0;
	REP(i,2*C) extra += duration[i];
	
	REP(i,C){
		int I1 = in_edge[i][0];
		int I2 = in_edge[i][1];
		int O1 = out_edge[i][0];
		int O2 = out_edge[i][1];
		
		int cost1 = INF, cost2 = INF;
		if(i == 0){
			cost1 = min(start[O1] + get_cost(I2, O2), start[O2] + get_cost(I1, O1));
			cost2 = min(start[O2] + get_cost(I2, O1), start[O1] + get_cost(I1, O2));
		} else {
			cost1 = get_cost(I1, O1) + get_cost(I2, O2);
			cost2 = get_cost(I1, O2) + get_cost(I2, O1);
		}
		
		extra += min(cost1, cost2);
		if(cost1 < cost2){
			next[I1] = O1;
			next[I2] = O2;
		} else {
			next[I2] = O1;
			next[I1] = O2;
		}
		
		switch_cost[i] = _abs(cost1 - cost2);
	}
	
//	cerr << extra << endl;
	
	int ans = get_mst() + extra;
	cout << ans << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int main(void){
	int TC,tc;
	cin >> TC;
	REP(tc,TC){
		printf("Case #%d: ", tc + 1);
		main2();
	}
	return 0;
}
