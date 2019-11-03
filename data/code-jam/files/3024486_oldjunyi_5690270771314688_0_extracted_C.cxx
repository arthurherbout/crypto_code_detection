#include <cstdio>
#include <vector>
#include <cstring>
#include <algorithm>
using namespace std;

const int INF = 1000000007;

char ty[1005];
int idx[1005];
int tag[1005];

vector<int> u;

int locate(int x, const vector<int>& u){
    return lower_bound(u.begin(),u.end(),x)-u.begin();
}

int dfs(int t, int n){
    //printf("dfs %d, ",t);
    //for(int i=1;i<=int(u.size());i++)
        //printf("%d ",tag[i]<0?2:tag[i]);
    //puts("");
    if(t==n){
        int m=u.size(),cnt=0;
        for(int x=1;x<=m;x++) if(tag[x]==1) cnt++;
        return cnt;
    }
    int ret=INF,x=idx[t];
    if(!x){
        if(ty[t]=='E'){
            int m=u.size();
            for(int x=1;x<=m+1;x++){
                if(tag[x]==1) continue;
                if(x>m) u.push_back(u.back()+1);
                int o=tag[x];
                tag[x]=1;
                ret=min(ret,dfs(t+1,n));
                tag[x]=o;
                if(x>m) u.pop_back();
            }
        }else{
            int m=u.size();
            for(int x=1;x<=m+1;x++){
                if(tag[x]==0) continue;
                if(x>m) u.push_back(u.back()+1);
                int o=tag[x];
                tag[x]=0;
                ret=min(ret,dfs(t+1,n));
                tag[x]=o;
                if(x>m) u.pop_back();
            }
        }
    }else{
        if(ty[t]=='E'){
            if(tag[x]==1) return INF;
            int o=tag[x];
            tag[x]=1;
            ret=min(ret,dfs(t+1,n));
            tag[x]=o;
        }else{
            if(tag[x]==0) return INF;
            int o=tag[x];
            tag[x]=0;
            ret=min(ret,dfs(t+1,n));
            tag[x]=o;
        }
    }
    return ret;
}

int main(){
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        int n;
        scanf("%d",&n);
        u.clear();
        memset(tag,-1,sizeof(tag));
        for(int i=0;i<n;i++){
            scanf("%s%d",ty+i,idx+i);
            u.push_back(idx[i]);
        }
        sort(u.begin(),u.end());
        u.erase(unique(u.begin(),u.end()),u.end());
        for(int i=0;i<n;i++) if(idx[i]) idx[i]=locate(idx[i],u)+1;
        int ans=dfs(0,n);
        fprintf(stderr,"gao test Case %d\n",no+1);
        printf("Case #%d: ",++no);
        if(ans>=INF){
            puts("CRIME TIME");
        }else{
            printf("%d\n",ans);
        }
    }
}
