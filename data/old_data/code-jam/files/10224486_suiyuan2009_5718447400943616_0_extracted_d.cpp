#include<cstdio>
#include<algorithm>
#include<string>
#include<iostream>
using namespace std;
typedef long long ll;
const int maxn=205;

int n;
char str[maxn][maxn];
int ret;

bool vis[maxn],visx[maxn];

bool cal(int x,int c) {

    bool sign=0;
    for(int i=0; i<n; i++)
        if(str[x][i]=='1'&&(!vis[i])) {
            sign=1;
            vis[i]=1;
            for(int j=0; j<n; j++)if(!visx[j]) {
                    visx[j]=1;
                    if(!cal(j,c+1))return 0;
                    visx[j]=0;
                }
            vis[i]=0;
        }
    if(!sign)return 0;
    return 1;
}

bool check() {
    for(int i=0; i<n; i++)vis[i]=visx[i]=0;
    for(int i=0; i<n; i++) {
        visx[i]=1;
        if(!cal(i,1))return 0;
        visx[i]=0;
    }
    return 1;
}

void dfs(int x,int y,int c) {
    if(x>=n) {
        if(check()) {
            if(ret==-1||ret>c)ret=c;
        }
        return;
    }
    int nx=x,ny=y+1;
    if(ny==n) {
        nx++;
        ny=0;
    }
    if(str[x][y]=='0') {
        str[x][y]='1';
        dfs(nx,ny,c+1);
        str[x][y]='0';
    }
    dfs(nx,ny,c);
}


int main() {
    freopen("D-small-attempt0.in","r",stdin);
    freopen("out.cpp","w",stdout);
    int T,ncase=0;
    scanf("%d",&T);
    while(T--) {
        ret=-1;
        scanf("%d",&n);
        for(int i=0; i<n; i++)scanf("%s",str[i]);
        dfs(0,0,0);
        printf("Case #%d: %d\n",++ncase,ret);
    }
    return 0;
}
