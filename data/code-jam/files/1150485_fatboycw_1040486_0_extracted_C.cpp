/*
 * Author: fatboy_cw
 * Created Time:  2011/5/22 1:40:25
 * File Name: C.cpp
 */
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
using namespace std;
#define SZ(v) ((int)(v).size())

const int maxn = 4005;
vector<int> poly[maxn];

int t,ca,ans;
int n,m,cnt;
int u[maxn],v[maxn],c[maxn];

bool find(int x,int u,int v){
    bool f1=false,f2=false;
    for(int i=0;i<poly[x].size();i++){
        if(poly[x][i]==u)   f1=true;
        if(poly[x][i]==v)   f2=true;
    }
    return f1 && f2;
}


void solve(int x,int u,int v){
    int start,end;
    for(int i=0;i<poly[x].size();i++){
        if(poly[x][i]==u){
            start=i;
        }
        if(poly[x][i]==v){
            end=i;
        }
    }
    vector<int> v1,v2;
    for(int i=start;;i=(i+1)%((int)poly[x].size())){
        v1.push_back(poly[x][i]);
        if(v1.back()==v)    break;
    }
    for(int i=end;;i=(i+1)%((int)poly[x].size())){
        v2.push_back(poly[x][i]);
        if(v2.back()==u)    break;
    }
    poly[x]=v1;
    poly[++cnt]=v2;
}

set<int> hash;

void color(){
    memset(c,-1,sizeof(c));
    for(int i=1;i<=cnt;i++){
        hash.clear();
        for(int j=1;j<=ans;j++) hash.insert(j);
        for(int j=0;j<poly[i].size();j++){
            int x=poly[i][j];
            if(hash.find(c[x])!=hash.end()){
                hash.erase(hash.find(c[x]));
            }
        }
        for(int j=0;j<poly[i].size();j++){
            int x=poly[i][j];
            if(c[x]==-1){
                if(!hash.empty()){
                    c[x]=*hash.begin();
                    hash.erase(hash.begin());
                }
            }
        }
    }
    for(int i=1;i<=n;i++)   if(c[i]==-1)    c[i]=1;
    printf("%d",c[1]);
    for(int i=2;i<=n;i++){
        printf(" %d",c[i]);
    }
    printf("\n");
}

int main() {
    freopen("C.out","w",stdout);
    scanf("%d",&t);
    while(t--){
        for(int i=0;i<maxn;i++) poly[i].clear();
        cnt=1;
        scanf("%d%d",&n,&m);
        for(int i=1;i<=m;i++)   scanf("%d",u+i);
        for(int i=1;i<=m;i++)   scanf("%d",v+i);
        for(int i=1;i<=n;i++)   poly[1].push_back(i);
        for(int i=1;i<=m;i++){
            for(int j=1;j<=cnt;j++){
                if(find(j,u[i],v[i])){
                    solve(j,u[i],v[i]);
                    break;
                }
            }
        }
        ans=1000000000;
        for(int i=1;i<=cnt;i++) ans=min(ans,(int)poly[i].size());
        printf("Case #%d: %d\n",++ca,ans);
        color();
    }
    return 0;
}

