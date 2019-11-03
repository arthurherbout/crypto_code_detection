#include <iostream>
#include <cstdio>
#include <vector>
#include <queue>
#include <algorithm>
#define F first
#define S second

using namespace std;

vector<int> g[51];

int used[51];

string ss[51];

struct comp{
bool operator ()(pair<string,int> p1,pair<string,int> p2){
    return p1.F>p2.F;
}
};

priority_queue<pair<string,int>,vector<pair<string,int> >,comp> bfs;

string hs;

void dfs (int x){
    if (used[x]) return;
    used[x]=1;
    hs+=ss[x];
    //cout <<x<<" "<<hs<<endl;
    vector<pair<string,int> > np;
    for (int nx:g[x]){
        np.push_back(make_pair(ss[nx],nx));
    }
    sort(np.begin(),np.end());
    for (auto asd:np){
        dfs (asd.S);
    }
}

int main(){
    freopen ("out.out","w",stdout);
    int tcs;
    cin>>tcs;
    for (int tc=1;tc<=tcs;tc++){
        cout <<"Case #"<<tc<<": ";
        int n,m;
        cin>>n>>m;
        for (int i=0;i<51;i++) g[i].clear();
        for (int i=1;i<=n;i++){
            cin>>ss[i];
        }
        for (int i=0;i<m;i++){
            int a,b;
            cin>>a>>b;
            g[a].push_back(b);
            g[b].push_back(a);
        }
        string mihs;
        for (int i=0;i<n;i++) mihs+="99999";
        for (int sp=1;sp<=n;sp++){
            for (int i=0;i<51;i++) used[i]=0;
            hs="";
            dfs (sp);
            //string hs;
            /*
            bfs.push(make_pair(ss[sp],sp));
            int f=0;
            while (!bfs.empty()){
                auto asd=bfs.top();
                bfs.pop();
                if (used[asd.S]) continue;
                used[asd.S]=1;
                f++;
                hs+=asd.F;
                for (int nx:g[asd.S]){
                    bfs.push(make_pair(ss[nx],nx));
                }
            }
            //cout <<hs<<endl;
            if (f==n) mihs=min(mihs,hs);
            */
            mihs=min(mihs,hs);
        }
        cout <<mihs<<endl;
    }
}
