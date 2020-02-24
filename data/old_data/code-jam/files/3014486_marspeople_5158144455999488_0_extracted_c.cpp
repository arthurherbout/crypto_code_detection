#include <cstdio>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <stack>
#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <set>
#include <queue>
using namespace std;
#define INF 0x3f3f3f3f
#define REP(i,n) for(int i=0; i<n; i++)
typedef long long int64;

int du[]={-1,0,0,1};
int dv[]={0,-1,1,0};

#define N 510
int w,h,b;
int t[N][N], us[N][N];

bool valid(int x, int y) {
    return (x>=0 && y>=0 && x<h && y<w && !t[x][y]);
}

void dfs(int x, int y) {
    if (!valid(x,y) || us[x][y]) return;
    us[x][y]=1;

    REP(k,4)
        dfs(x+du[k],y+dv[k]);
}

int gm(vector<int> &v, int p) {
    if (p>=v.size()) return INF;
    return v[p];
}

int main() {
    int nt;

    scanf("%d",&nt);
    REP(ct,nt) {
        scanf("%d%d%d",&w,&h,&b);

        memset(t,0,sizeof(t));
        REP(i,b) {
            int x1,y1,x2,y2;
            scanf("%d%d%d%d",&y1,&x1,&y2,&x2);
            for (int x=x1;x<=x2;x++)
                for (int y=y1;y<=y2;y++)
                    t[x][y]=1;
        }

        memset(us,0,sizeof(us));
        vector<int> f;
        REP(i,w) {
            dfs(h-1,i);
            f.push_back(i);
        }

        REP(z,h) {
            vector<int> nf,wa,cd;
            REP(j,w) {
                if (t[z][j]==1)
                    wa.push_back(j);
                if (us[z][j] && (z==h-1 || us[z+1][j])) cd.push_back(j);
            }

            int hi=-1,hk=-1;

            for (int i=0,j=0,k=0; ; ) {
                if (j<wa.size() && wa[j]<=gm(f,i) && wa[j]<=gm(cd,k)) {
                    while (i<f.size() && f[i]<=wa[j]) i++;
                    while (k<cd.size() && cd[k]<=wa[j]) k++;
                    j++;
                    hk=hi=-1;
                }
                else if (i<f.size() && f[i]<gm(wa,j) && f[i]<=gm(cd,k)) {
                    if (hk!=-1) nf.push_back(hk);
                    else hi=f[i];
                    i++;
                    hk=-1;
                }
                else if (k<cd.size() && cd[k]<gm(wa,j) && cd[k]<=gm(f,i)) {
                    if (hi!=-1) nf.push_back(cd[k]);
                    else hk=cd[k];
                    k++;
                    hi=-1;
                }
                else break;
            }
            f=nf;
        }

        printf("Case #%d: %d\n",ct+1,(int)f.size());
    }

    return 0;
}

