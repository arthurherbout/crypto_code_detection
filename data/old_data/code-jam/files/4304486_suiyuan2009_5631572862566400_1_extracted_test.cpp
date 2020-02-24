#include<cstdio>
#include<algorithm>
#include<vector>
#include<string>
#include<iostream>
#include<cstring>
using namespace std;
const int maxn=1005;
typedef long long ll;

int a[maxn],n,d[maxn],b[maxn];
int ti[maxn];
vector<int>g[maxn];
int ret;
void dfs(int u) {
    if(d[a[u]]) {
        if(ti[a[u]]==ti[u])
            ret=max(ret,d[u]-d[a[u]]+1);
    } else {
        d[a[u]]=d[u]+1;
        ti[a[u]]=ti[u];
        dfs(a[u]);
    }
}

int dfs_2(int u) {
    if(g[u].size()==0)return 1;
    int mx=1;
    for(int i=0; i<g[u].size(); i++) {
        int v=g[u][i];
        if(!b[v])mx=max(mx,dfs_2(v)+1);
    }
    return mx;
}

int main() {
    freopen("C-large.in","r",stdin);
    freopen("out2","w",stdout);
    int T,ncase=0;
    scanf("%d",&T);
    while(T--) {
        memset(d,0,sizeof(d));
        memset(b,0,sizeof(b));
        for(int i=0; i<maxn; i++)g[i].clear();
        memset(ti,0,sizeof(ti));
        scanf("%d",&n);
        for(int i=0; i<n; i++) {
            scanf("%d",&a[i]);
            a[i]--;
            g[a[i]].push_back(i);
        }
        int cnt=0;
        for(int i=0; i<n; i++)
            if(a[a[i]]==i) {
                cnt++;
                b[i]=b[a[i]]=1;
            }
        //printf("******* %d\n",cnt);
        for(int i=0; i<n; i++)
            if(b[i])cnt+=dfs_2(i)-1;
        ret=1;
        for(int i=0; i<n; i++)
            if(!d[i]) {
                ti[i]=i+1;
                d[i]=1;
                dfs(i);
            }
        printf("Case #%d: %d\n",++ncase,max(ret,cnt));
    }
    return 0;
}
