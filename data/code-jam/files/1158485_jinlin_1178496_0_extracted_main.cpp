#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <map>

using namespace std;


struct Edge {
	Edge* nxt;
	int tar;
} edges[20010], *ed;

struct Node {
	Edge* fst;
	int visited;
} nodes[10010];

int n, m;
void con(int i1, int j1, int i2, int j2) {
	int u = i1*m+j1;
	int v = i2*m+j2;
	//printf("!!: con %d %d\n", u, v);
	ed->tar = v;
	ed->nxt = nodes[u].fst;
	nodes[u].fst = ed++;
	swap(u, v);
	ed->tar = v;
	ed->nxt = nodes[u].fst;
	nodes[u].fst = ed++;
}

const int MOD = 1000003ll;
int cntV, cntE;
char s[200];

void dfs(int cur) {
	//printf("!!: (%d\n", cur);
	if(nodes[cur].visited) return;
	//printf("!!:  %d)\n", cur);
	++cntV;
	nodes[cur].visited = 1;
	for(Edge* e=nodes[cur].fst; e; e=e->nxt) {
		++cntE;
		dfs(e->tar);
	}
}

int main() {
	int T;
	scanf("%d", &T);
	for(int cas=1; cas<=T; ++cas) {
		scanf("%d%d", &n, &m);
		memset(nodes, 0, sizeof(nodes));
		ed = edges;
		for(int i=0; i!=n; ++i) {
			scanf("%s", s);
			for(int j=0; j!=m; ++j) {
				if(s[j]=='|')
					con((i+n-1)%n,	j,		(i+1)%n,	j);
				if(s[j]=='-')
					con(i,		(j+m-1)%m, 	i,		(j+1)%m);
				if(s[j]=='/')
					con((i+n-1)%n,	(j+1)%m,	(i+1)%n,	(j+m-1)%m);
				if(s[j]=='\\')
					con((i+n-1)%n,	(j+m-1)%m,	(i+1)%n,	(j+1)%m);
			}
		}
		int ans=1;
		for(int i=0; i!=n*m; ++i) {
			if(nodes[i].visited) continue;
			cntV = cntE = 0;
			dfs(i);
			//printf("!!: %d %d\n", cntV, cntE);
			if(cntE/2==cntV) ans = (ans<<1)%MOD;
			else ans = 0;
		}
		printf("Case #%d: %d\n", cas, ans);
	}
}
