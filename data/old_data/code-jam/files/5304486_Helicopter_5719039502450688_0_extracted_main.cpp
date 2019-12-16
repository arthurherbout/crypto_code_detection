#include<time.h>
#include<cstdio>
#include<iostream>
#include<algorithm>
#include<cstring>
#include<cmath>
#include<map>
#include<queue>
#define maxn 1101
#define MAXN 120
#define inf (1<<30)
#define modp 1000000007

#define fst first
#define scn second
using namespace std;

typedef long long LL;
int n,m,tot;

LL mpow(LL x, LL y)
{
    if (y==0) return 1;
    LL res=mpow(x,y>>1);
    res*=res;
    res%=modp;
    if (y&1) res*=x;
    return res%modp;
}

typedef pair<double,double> PNT;
typedef pair<LL, LL> PA;

typedef struct Ed{
    int l,r,nxt;
}E;

const double pi=4*atan(1);
const double eps = 1e-6;

char upc(char x)
{
    if (x>='a'&&x<='z') return x-32; else return x;
}

int mcmp(const E& x, const E&y)
{
    return x.nxt<y.nxt;
}


PA solve(LL N, LL K) {
    if (K == 1) {
        return PA(N>>1,N-1>>1);
    }
    LL pre = (K>>1);
    PA tmp = solve(N,pre);
    if (K == (pre<<1)) {
        N = tmp.first;
        return PA(N>>1,N-1>>1);
    } else {
        N = tmp.second;
        return PA(N>>1,N-1>>1);
    }
}

int Hd,Ad,Hk,Ak,B,D;

typedef pair<PA,PA> QUA;

int bfs() {
    queue<QUA> q;
    map<QUA,int> mp;
    QUA s=QUA(PA(Hd,Ad),PA(Hk,Ak));
    while (!q.empty()) q.pop();
    q.push(s);
    mp.clear();
    mp[s]=0;
    while (!q.empty()) {
        QUA tmp=q.front();
        q.pop();
        int step=mp[tmp];
        int H1=tmp.fst.fst;
        int A1=tmp.fst.scn;
        int H2=tmp.scn.fst;
        int A2=tmp.scn.scn;
        QUA t;
        //action1
        t=QUA(PA(H1-A2,A1),PA(H2-A1,A2));
        if (H2-A1<=0) return step+1;
        if (H1-A2>0) {
            if (mp.find(t)==mp.end()||mp[t]>step+1) {
                q.push(t);
                mp[t]=step+1;
            }
        }
        //action2
        t=QUA(PA(H1-A2,A1+B),PA(H2,A2));
        if (H1-A2>0&&A1<100) {
            if (mp.find(t)==mp.end()||mp[t]>step+1) {
                q.push(t);
                mp[t]=step+1;
            }
        }
        //action3
        t=QUA(PA(Hd-A2,A1),PA(H2,A2));
        if (Hd-A2>0) {
            if (mp.find(t)==mp.end()||mp[t]>step+1) {
                q.push(t);
                mp[t]=step+1;
            }
        }
        //action4
        t=QUA(PA(H1-A2,A1),PA(H2,max(A2-D,0)));
        if (H1-A2>0) {
            if (mp.find(t)==mp.end()||mp[t]>step+1) {
                q.push(t);
                mp[t]=step+1;
            }
        }

    }
    return -1;
}

int main(void)
{
    int T = 1;
    char op;
//    freopen("in.txt","r",stdin);
//    freopen("out.txt","w",stdout);
    scanf("%d",&T);
    for (int ci = 1; ci <= T; ++ci)
    //while (scanf("%d",&n)!=EOF)
    {
        scanf("%d%d%d%d%d%d",&Hd,&Ad,&Hk,&Ak,&B,&D);
        int ans=bfs();
        printf("Case #%d: ",ci);
        if (ans>=0) printf("%d\n",ans);
        else puts("IMPOSSIBLE");
    }
    return 0;
}
