#include <cstdio>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

// in: n, m, graph
// out: match, matched
// vertex cover: (reached[0][left_node] == 0) || (reached[1][right_node] == 1)
struct BipartiteMatching
{
  int n, m;
  vector<vector<int> > graph;
  vector<int> matched, match;
  vector<int> edgeview;
  vector<int> level;
  vector<int> reached[2];
  BipartiteMatching(int n, int m) : n(n), m(m), graph(n), matched(m, -1), match(n, -1) {}

  bool assignLevel()
  {
    bool reachable = false;
    level.assign(n, -1);
    reached[0].assign(n, 0);
    reached[1].assign(m, 0);
    queue<int> q;
    for (int i = 0; i < n; i++) {
      if (match[i] == -1) {
        level[i] = 0;
        reached[0][i] = 1;
        q.push(i);
      }
    }
    while (!q.empty()) {
      auto cur = q.front(); q.pop();
      for (auto adj : graph[cur]) {
        reached[1][adj] = 1;
        auto next = matched[adj];
        if (next == -1) {
          reachable = true;
        }
        else if (level[next] == -1) {
          level[next] = level[cur] + 1;
          reached[0][next] = 1;
          q.push(next);
        }
      }
    }
    return reachable;
  }

  int findpath(int nod) {
    for (int &i = edgeview[nod]; i < graph[nod].size(); i++) {
      int adj = graph[nod][i];
      int next = matched[adj];
      if (next >= 0 && level[next] != level[nod] + 1) continue;
      if (next == -1 || findpath(next)) {
        match[nod] = adj;
        matched[adj] = nod;
        return 1;
      }
    }
    return 0;
  }

  int solve()
  {
    int ans = 0;
    while (assignLevel()) {
      edgeview.assign(n, 0);
      for (int i = 0; i < n; i++)
        if (match[i] == -1)
          ans += findpath(i);
    }
    return ans;
  }
};


int main(){
	int T; scanf("%d", &T);
	for(int ti=1;ti<=T;ti++){
		fprintf(stderr, "start test case %d\n", ti);
		int n, k; scanf("%d%d", &n, &k);
		vector<vector<int>> a(n, vector<int>(n, 0)),
			b(n, vector<int>(n, 0));
		for(int i=0;i<k;i++){
			char t; scanf(" %c", &t);
			int li, lj; scanf("%d%d", &li, &lj);
			if(t == 'x') a[li-1][lj-1] = 1;
			else if(t == '+') b[li-1][lj-1] = 1;
			else a[li-1][lj-1] = b[li-1][lj-1] = 1;
		}
		/* B */
		vector<vector<int>> ans_a(n, vector<int>(n, 0));
		vector<int> oc0(n,0), oc1(n,0);
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(a[i][j]){
					oc0[i] = oc1[j] = 1;
					ans_a[i][j] = 1;
				}
			}
		}
		for(int i=0;i<n;i++){
			if(!oc0[i]){
				for(int j=0;j<n;j++){
					if(!oc1[j]){
						oc0[i] = oc1[j] = 1;
						ans_a[i][j] = 1;
						break;
					}
				}
			}
		}
		/* B */
		BipartiteMatching bm(2*n-1, 2*n-1);
		vector<int> occ0(2*n-1,0), occ1(2*n-1,0);
		vector<vector<int>> num0(n, vector<int>(n, -1)),
			num1(n, vector<int>(n, -1)),
			vis(2*n-1, vector<int>(2*n-1, 0));
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				num0[i][j] = i + j;
				num1[i][j] = n - 1 + j - i;
			}
		}
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(b[i][j]){
					occ0[num0[i][j]] = occ1[num1[i][j]] = 1;
					bm.graph[num0[i][j]].push_back(num1[i][j]);
					vis[num0[i][j]][num1[i][j]] = 1;
				}
			}
		}
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(!vis[num0[i][j]][num1[i][j]]){
					vis[num0[i][j]][num1[i][j]] = 1;
					if(!occ0[num0[i][j]] && !occ1[num1[i][j]]){
						bm.graph[num0[i][j]].push_back(num1[i][j]);
					}
				}
			}
		}
		int ans = bm.solve() + n;
		vector<vector<int>> ans_b(n, vector<int>(n, 0));
		for(int k=0;k<2*n-1;k++){
			if(bm.match[k] != -1){
				for(int i=0;i<n;i++){
					for(int j=0;j<n;j++){
						if(num0[i][j] == k && num1[i][j] == bm.match[k]){
							ans_b[i][j] = 1;
						}
					}
				}
			}
		}
		/*
		printf("\n");
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++) printf("%d", ans_a[i][j]);
			printf("\n");
		}
		printf("\n");
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++) printf("%d", ans_b[i][j]);
			printf("\n");
		}
		*/
		printf("Case #%d: ", ti);
		int cnt = 0;
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(a[i][j] != ans_a[i][j] || b[i][j] != ans_b[i][j]){
					cnt++;
				}
			}
		}
		printf("%d %d\n", ans, cnt);
		for(int i=0;i<n;i++){
			for(int j=0;j<n;j++){
				if(a[i][j] != ans_a[i][j] || b[i][j] != ans_b[i][j]){
					char cc;
					if(ans_a[i][j] && ans_b[i][j]) cc = 'o';
					else if(ans_a[i][j]) cc = 'x';
					else cc = '+';
					printf("%c %d %d\n", cc, i+1, j+1);
				}
			}
		}
	}
	return 0;
}
