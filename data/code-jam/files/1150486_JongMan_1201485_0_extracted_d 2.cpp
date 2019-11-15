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
int n, m;
const int MAXN = 40;
vector<int> adj[MAXN];
int adjmat[MAXN][MAXN];
int common[MAXN][MAXN];
int common3[MAXN][MAXN][MAXN];

int c[MAXN];
int cache[MAXN][MAXN];

int dp(int here, int last) {
	if(c[here] == 1) return 0;
	int& ret = cache[here][last];
	if(ret != -1) return ret;
	ret = 0;
	for(int next = 0; next < n; next++) 
		if(c[next] == c[here] - 1 && adjmat[next][here]) {
			int add = 0;
			if(here != last) {
				add = adj[next].size() + common3[next][here][last] 
				- common[next][here]
				- common[next][last];
			}
			else {
				add = adj[next].size() - common[here][last];
				
			}
			ret = max(ret, dp(next, here) + add);
		}
	return ret;
}

void solve() {	
	memset(adjmat, 0, sizeof(adjmat));
	for(int i = 0; i < n; i++) 
		for(int j = 0; j < adj[i].size(); j++)
			adjmat[i][adj[i][j]] = 1;
	memset(common, 0, sizeof(common));
	memset(common3, 0, sizeof(common3));
	for(int i = 0; i < n; i++) 
		for(int j = 0; j < n; j++) {
			for(int k = 0; k < n; k++) {
				if(adjmat[i][k] == 1 && adjmat[j][k] == 1)
					++common[i][j];
				for(int l = 0; l < n; l++) 
					if(adjmat[i][l] && adjmat[j][l] && adjmat[k][l])
						++common3[i][j][k];
			}
		}
	memset(c, -1, sizeof(c));
	c[0] = 0;
	queue<int> q;
	q.push(0);
	while(!q.empty()) {
		int here = q.front();
		q.pop();
		for(int i = 0; i < adj[here].size(); i++) {
			int there = adj[here][i];
			if(c[there] == -1) {
				c[there] = c[here] + 1;
				q.push(there);
			}
		}
	}
	memset(cache, -1, sizeof(cache));
	int threaten = dp(1, 1) + adj[1].size();
	if(c[1] > 1) threaten--;
	printf("%d %d\n", c[1]-1, threaten);
}

int main() {
    int cases;
    cin >> cases;
    for(int cc = 0; cc < cases; ++cc) {
    	cin >> n >> m;
    	for(int i = 0; i < n; i++) adj[i].clear();
    	memset(adjmat, 0, sizeof(adjmat));
    	for(int i = 0; i < m; i++) {
    		int a, b;
    		scanf("%d,%d", &a, &b);
    		if(a <= 1) a = 1 - a;
    		if(b <= 1) b = 1 - b;
    		adj[a].push_back(b);
    		adj[b].push_back(a);
    	}
    	printf("Case #%d: ", cc+1);
    	solve();
    }
}

