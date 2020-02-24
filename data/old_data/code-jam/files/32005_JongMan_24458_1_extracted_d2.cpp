#include<iostream>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

int n, m;
bool small[110][110], large[110][110];
int cache[110][110][110];
int parent[110];
int sizeLargeCache[110];
int sizeSmallCache[110][110];

const int MAXV = 210;
struct NetworkFlow
{
	int flow[MAXV][MAXV], cap[MAXV][MAXV], totalFlow, V;
	NetworkFlow(int V): V(V) { CLEAR(cap,0); CLEAR(flow,0);totalFlow = 0; }

	void addEdge(int a, int b, int c) { cap[a][b] += c; }

	void push(int a, int b, int c) { flow[a][b] += c; flow[b][a] = - flow[a][b]; }
	int res(int a, int b) { return cap[a][b] - flow[a][b]; }

	int pushFlow(int source = 0, int sink = 1)
	{
		vector<int> pred(V, -1); queue<int> q;
		q.push(source); pred[source] = source;
		while(!q.empty() && pred[sink] == -1)
		{
			int u = q.front(); q.pop();
			for(int v = 0; v < V; ++v) if(res(u,v) > 0 && pred[v] == -1) { pred[v] = u; q.push(v); }
		}
		if(pred[sink] == -1) return 0;
		int h, amt = 987654321;
		h = sink; while(pred[h] != h) { amt = min(amt, res(pred[h], h)); h = pred[h]; }
		h = sink; while(pred[h] != h) { push(pred[h], h, amt); h = pred[h]; }
		totalFlow += amt;
		return amt;
	}
};


void dfs(int u, int p = -2)
{
	parent[u] = p;
	REP(i,n) if(large[u][i] && parent[i] == -1)
		dfs(i, u);
}

int sizeLarge(int u)
{
	int& ret = sizeLargeCache[u];
	if(ret != -1) return ret;
	ret = 0;
	REP(i,n) if(large[u][i] && parent[i] == u)
		ret += sizeLarge(i);
	return ret;
}

int sizeSmall(int u, int parent)
{
	int& ret = sizeSmallCache[u][parent+1];
	if(ret != -1) return ret;
	ret = 0;
	REP(v,m) if(small[u][v] && parent != v)
		ret += sizeSmall(v, u);
	return ret;
}

const int SRC = 1;
const int SNK = 0;

int canMatch(int i, int j, int jparent = -1)
{
	int& ret = cache[i][j][jparent+1];
	if(ret != -1) return ret;
	ret = 0;
	int A = sizeLarge(i), B = sizeSmall(j, jparent);
	if(A < B) return ret = 0;
	A = B = 0;
	REP(u,n) if(parent[u] == i) ++A;
	REP(u,m) if(small[j][u] && jparent != u) ++B;
	if(A < B) return ret = 0;
	NetworkFlow* nf = new NetworkFlow(A+B+2);
	int a = 0, b = 0;
	REP(t,A) nf->addEdge(SRC, 2+t, 1);
	REP(t,B) nf->addEdge(2+A+t, SNK, 1);
	REP(i2,n) if(parent[i2] == i)
	{
		b = 0;
		REP(j2,m) if(small[j2][j] && jparent != j2)
		{

			if(canMatch(i2, j2, j))
			{
				nf->addEdge(2+a, 2+A+b, 1);
			//	printf("matching (%d,%d) jparent %d ::: %d matches to %d\n", i,j,jparent,i2,j2);
			}
			b++;
		}
		++a;
	}
	while(nf->pushFlow(SRC, SNK));	
	if(nf->totalFlow >= B)
		ret = 1;
	delete nf;
//	printf("match %d %d (jparent %d) = %d\n", i, j, jparent, ret);
	return ret;
}

int main()
{
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		CLEAR(sizeLargeCache,-1);
		CLEAR(sizeSmallCache,-1);
		CLEAR(parent,-1);
		CLEAR(cache,-1);
		fprintf(stderr, "%d of %d ..\r", cc+1, cases);
		printf("Case #%d: ", cc+1);
		cin >> n;
		CLEAR(small,0); CLEAR(large,0);
		REP(i,n-1)
		{
			int a, b;
			cin >> a >> b;
			--a; --b;
			large[a][b] = large[b][a] = true;
		}
		cin >> m;
		REP(i,m-1)
		{
			int a, b;
			cin >> a >> b;
			--a; --b;
			small[a][b] = small[b][a] = true;
		}
		dfs(0);
		bool ok = false;
		REP(i,n) if(!ok) REP(j,m) if(canMatch(i, j)) { ok = true; break; }
//		canMatch(0, 1);		
		if(ok) 
			printf("YES\n");
		else
			printf("NO\n");
	}
}
