#include <cstdio>
#include <vector>
#include <cstring>
#include <map>
#include <algorithm>
using namespace std;

const int INF = 1000000007;

map<int,int> dp[4005][5];

class TrieGraph {
public:
    static const int SIZE = 1130005;
    static const int LEAF = 26;
    vector<int> arc[SIZE];
    int tag[SIZE],e[SIZE][LEAF],n;
    void init(){
        memset(e,-1,sizeof(e));
        n=1;
    }
    void insert(const char* s){
        int x=0;
        for(int i=0;s[i];i++){
            int c=s[i]-'a';
            if(e[x][c]<0){
                arc[x].push_back(n);
                x=e[x][c]=n++;
            }else{
                x=e[x][c];
            }
        }
        tag[x]=1;
    }
}ac;

void init(){
    char rs[200];
    FILE* fp=fopen("garbled_email_dictionary.txt","r");
    ac.init();
    while(fgets(rs,200,fp)){
        rs[strlen(rs)-1]=0;
        if(!strcmp(rs,"codejam")) puts("has codejam!");
        ac.insert(rs);
    }
    fclose(fp);
}

void relax(map<int,int>& u, int idx, int use){
    auto t=u.find(idx);
    if(t==u.end()) u[idx]=use;
    else if(use<t->second) t->second=use;
}

int main(){
    init();
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        char s[4005];
        scanf("%s",s);
        dp[0][4][0]=0;
        int n=strlen(s),ans=INF;
        fprintf(stderr,"gao Case %d\n",no);
        for(int i=0;i<n;i++){
            fprintf(stderr,"%d ",i);
            for(int c=0;c<5;c++){
                for(auto& x:dp[i][c]){
                    int src=x.first,use=x.second;
                    int dst=ac.e[src][s[i]-'a'];
                    if(~dst){
                        relax(dp[i+1][min(c+1,4)],dst,use);
                        if(ac.tag[dst]) relax(dp[i+1][min(c+1,4)],0,use);
                    }
                    if(c==4){
                        for(auto idx:ac.arc[src]){
                            relax(dp[i+1][0],idx,use+1);
                            if(ac.tag[idx]) relax(dp[i+1][0],0,use+1);
                        }
                    }
                }
                dp[i][c].clear();
            }
        }
        fprintf(stderr,"\n\n");
        for(int c=0;c<5;c++){
            auto t=dp[n][c].find(0);
            if(t!=dp[n][c].end()) ans=min(ans,t->second);
            dp[n][c].clear();
        }
        printf("Case #%d: %d\n",++no,ans);
    }
}
