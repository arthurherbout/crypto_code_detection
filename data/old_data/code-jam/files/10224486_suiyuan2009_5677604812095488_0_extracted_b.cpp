#include<cstdio>
#include<algorithm>
#include<string>
#include<iostream>
using namespace std;
typedef long long ll;
const int maxn=205;

int n,m;
double p[maxn],ret,tmpret;

int path[maxn];

void cal(int pos,int c1,int c2,double ans) {
    if(c1+c1>m||c2+c2>m)return;
    if(pos==m) {
        if(c1+c1==m) {
            tmpret+=ans;
            //printf("** %lf\n",ans);
        }
        return;
    }
    if(c2+c2<m)cal(pos+1,c1,c2+1,ans*(1-p[path[pos]]));
    if(c1+c1<m)cal(pos+1,c1+1,c2,ans*p[path[pos]]);
}

void pd() {
    tmpret=0;
    cal(0,0,0,1);
    if(tmpret>ret)ret=tmpret;
}

void dfs(int pos,int c) {
    if(c+n-pos<m)return;
    if(pos==n) {
        if(c==m) {
            pd();
        }
        return;
    }
    if(c<m) {
        path[c]=pos;
        dfs(pos+1,c+1);
    }
    dfs(pos+1,c);
}

int main() {
    freopen("B-small-attempt0.in","r",stdin);
    freopen("out.cpp","w",stdout);
    int T,ncase=0;
    scanf("%d",&T);
    while(T--) {
        scanf("%d%d",&n,&m);
        for(int i=0; i<n; i++)scanf("%lf",&p[i]);
        ret=0;
        dfs(0,0);
        printf("Case #%d: %.10f\n",++ncase,ret);
    }
    return 0;
}
