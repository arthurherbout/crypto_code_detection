#include <cstdio>
#include <string>
#include <vector>
#include <cstring>
#include <set>
#include <queue>
#include <algorithm>
using namespace std;

using LL = long long;
const int INF = 1000000007;

string a[55];
vector<int> e[55];
int pos,order[55];

void dfs(int x){
    vector<int> u=e[x];
    while(1){
        auto p=find(u.begin(),u.end(),order[pos]);
        if(p==u.end()) return;
        u.erase(p);
        dfs(order[pos++]);
    }
}

int main(){
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        int n,m;
        scanf("%d%d",&n,&m);
        for(int i=0;i<n;i++){
            char str[9];
            scanf("%s",str);
            a[i]=str;
            order[i]=i;
        }
        for(int i=0;i<m;i++){
            int x,y;
            scanf("%d%d",&x,&y);
            --x,--y;
            e[x].push_back(y);
            e[y].push_back(x);
        }
        string ans="~";
        order[n]=-1;
        do{
            pos=1;
            dfs(order[0]);
            if(pos==n){
                string now;
                for(int i=0;i<n;i++) now+=a[order[i]];
                ans=min(ans,now);
            }
        }while(next_permutation(order,order+n));
        printf("Case #%d: %s\n",++no,ans.c_str());
        for(int i=0;i<n;i++) e[i].clear();
    }
}
