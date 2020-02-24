#include<cstdio>
#include<algorithm>
#include<string>
#include<iostream>
using namespace std;
typedef long long ll;
const int maxn=12;

int n,p,r,s;

bool check(int x,int y,int z,int depth) {
    if(depth==n) {
        return x==p&&y==r&&z==s;
    }
    if(check(x+z,y+x,y+z,depth+1))return true;
    return false;
}

string dfs(int x,int depth) {
    if(depth==n) {
        if(x==0)return "P";
        else if(x==1)return "R";
        return "S";
    }
    string l,r;
    if(x==0) {
        l=dfs(0,depth+1);
        r=dfs(1,depth+1);
    } else if(x==1) {
        l=dfs(1,depth+1);
        r=dfs(2,depth+1);
    } else {
        l=dfs(2,depth+1);
        r=dfs(0,depth+1);
    }
    if(l>r)return r+l;
    return l+r;
}

string ret;

int main() {
    freopen("A-large.in","r",stdin);
    freopen("out.cpp","w",stdout);
    int T,ncase=0;
    scanf("%d",&T);
    while(T--) {
        scanf("%d%d%d%d",&n,&r,&p,&s);
        ret="";
        if(check(1,0,0,0)) {
            //printf("**\n");
            string ans=dfs(0,0);
            if(ret==""||ret>ans)ret=ans;
        }
        if(check(0,1,0,0)) {
            //printf("**\n");
            string ans=dfs(1,0);
            if(ret==""||ret>ans)ret=ans;
        }
        if(check(0,0,1,0)) {
            //printf("**\n");
            string ans=dfs(2,0);
            if(ret==""||ret>ans)ret=ans;
        }
        if(ret=="")printf("Case #%d: IMPOSSIBLE\n",++ncase);
        else {
            printf("Case #%d: ",++ncase);
            cout<<ret<<endl;
        }
    }
    return 0;
}
