#include <cstdio>
#include <vector>
#include <algorithm>
using namespace std;

typedef vector<int> vi;
typedef vector<vi> vvi;

int n;
vvi g, g_rev;
int node_except;

int dfs(int here){
    int ret = 0;
    for(int there : g_rev[here]){
        if(there == node_except) continue;
        ret = max(ret, 1 + dfs(there));
    }
    return ret;
}


int max_circle(int node){
    int here = g[node][0];
    vi visited(n, 0);
    visited[node] = true;
    int prev = node;
    int len = 1;
    while(!visited[here]){
        visited[here] = true;
        len++;
        prev = here;
        here = g[here][0];
    }
    if(here == node){
        return len;
    }
    else return 0;
}

int main(){
    int T; scanf("%d", &T);
    for(int ti=1;ti<=T;ti++){
        fprintf(stderr, "start %d\n", ti);
        scanf("%d", &n);
        g = vvi(n), g_rev = vvi(n);
        for(int i=0;i<n;i++){
            int j; scanf("%d", &j); j--;
            g[i].push_back(j);
            g_rev[j].push_back(i);
        }
        int ans = 0;
        for(int i=0;i<n;i++){
            ans = max(ans, max_circle(i));
        }
        int sum = 0;
        vi t(n, 0);
        for(int i=0;i<n;i++){
            if(t[i]) continue;
            if(g[g[i][0]][0] == i){
                int node1 = i;
                int node2 = g[i][0];
                node_except = node2;
                sum += dfs(node1);
                node_except = node1;
                sum += dfs(node2);
                sum += 2;
                t[node1] = t[node2] = 1;
            }
        }
        ans = max(ans, sum);
        printf("Case #%d: %d\n", ti, ans);
        fprintf(stderr, "end %d\n", ti);
    }
    return 0;
}
