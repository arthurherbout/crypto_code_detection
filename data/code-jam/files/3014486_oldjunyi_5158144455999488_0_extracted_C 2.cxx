#include <cstdio>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

#include <cstring>
#include <queue>
#include <algorithm>
using namespace std;

class Network {
public:
    typedef int VAL;
    static const int SIZE = 105*500*2;
    static const int INF = 1000000007;
    typedef struct ARC{int t,c; VAL w; ARC* o;}* PTR;
    ARC arc[625000];
    PTR now[SIZE],e[SIZE];
    int cnt[SIZE],l[SIZE],r[SIZE],edge;
    VAL sum;
    void clear(){memset(e,edge=sum=0,sizeof(e));}
    ARC& REV(PTR x){return arc[(x-arc)^1];}
    int flow(int S, int T){return improved_sap(S,T,INF);}
    PTR add_edge(int x, int y, int c, VAL w = 0){
        e[x]=&(arc[edge++]=(ARC){y,c,+w,e[x]});
        e[y]=&(arc[edge++]=(ARC){x,0,-w,e[y]});
        return e[x];
    }
    int aug(int S, int T, int& can){
        int x,z=T,use=can;
        for(x=S;x!=T;x=now[x]->t) if(use>now[x]->c) use=now[z=x]->c;
        for(x=S;x!=T;x=now[x]->t){
                now[x]->c-=use;
            REV(now[x]).c+=use;
            sum+=use*now[x]->w;
        }
        can-=use;
        return z;
    }
    int improved_sap(int S, int T, int can){
        if(S==T) return can;
        int in=can,x,m;
        memcpy(now,e,sizeof(now));
        memset(cnt,0,sizeof(cnt));
        fill_n(l,SIZE,int(SIZE));
        for(int i=m=l[r[0]=T]=0;i<=m;i++){
            cnt[l[x=r[i]]]++;
            for(PTR u=e[x];u;u=u->o)
                if(l[u->t]==SIZE && REV(u).c) l[r[++m]=u->t]=l[x]+1;
        }
        for(x=r[S]=S;l[S]!=SIZE;x=r[x]){
        JMP:for(PTR& u=now[x];u;u=u->o) if(l[u->t]<l[x] && u->c){
                r[u->t]=x;
                x=u->t==T?aug(S,T,can):u->t;
                if(x==T) return in; else goto JMP;
            }
            if(!--cnt[l[x]]) break; else l[x]=SIZE;
            for(PTR u=e[x];u;u=u->o)
                if(l[u->t]+1<l[x] && u->c) now[x]=u,l[x]=l[u->t]+1;
            if(l[x]!=SIZE) cnt[l[x]]++;
        }
        return in-can;
    }
}net;

int idx[105][505];

const int mx[4] = {0,1,0,-1};
const int my[4] = {-1,0,1,0};

int main(){
    int cs,no=0;
    scanf("%d",&cs);
    while(cs--){
        net.clear();
        int w,h,n;
        scanf("%d%d%d",&w,&h,&n);
        memset(idx,1,sizeof(idx));
        for(int i=0;i<n;i++){
            int x1,y1,x2,y2;
            scanf("%d%d",&x1,&y1);
            scanf("%d%d",&x2,&y2);
            for(int x=x1;x<=x2;x++)
            for(int y=y1;y<=y2;y++) idx[x][y]=0;
        }
        int S=0,T=1,cnt=0;
        for(int x=0;x<w;x++) for(int y=0;y<h;y++){
            if(!idx[x][y]) continue;
            idx[x][y]=cnt+=2;
            net.add_edge(idx[x][y],idx[x][y]+1,1);
            if(y==  0) net.add_edge(S,idx[x][y],1);
            if(y==h-1) net.add_edge(idx[x][y]+1,T,1);
        }
        for(int x=0;x<w;x++) for(int y=0;y<h;y++){
            for(int o=0;o<4;o++){
                int dx=x+mx[o];
                int dy=y+my[o];
                if(dx<0 || dx>=w
                || dy<0 || dy>=h
                || !idx[dx][dy]) continue;
                net.add_edge(idx[x][y]+1,idx[dx][dy],1);
            }
        }
        printf("Case #%d: %d\n",++no,net.flow(S,T));
    }
}
