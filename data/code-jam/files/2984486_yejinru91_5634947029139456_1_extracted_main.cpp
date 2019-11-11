#include <set>
#include <map>
#include <list>
#include <cmath>
#include <queue>
#include <stack>
#include <string>
#include <vector>
#include <cstdio>
#include<iomanip>
#include <cstring>
#include <complex>
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace std;

typedef long long ll;
typedef unsigned long long ull;

#define debug puts("yejinru")
#define rep(i,n) for(int i=0;i<n;i++)
#define rep1(i,n) for(int i=1;i<=n;i++)
#define REP(i,a,b) for(int i=a;i<=b;i++)
#define foreach(i,vec) for(int i=0;i<(int)vec.size();i++)
#define pb push_back
#define RD(n) scanf("%d",&n)
#define RD2(x,y) scanf("%d%d",&x,&y)
#define RD3(x,y,z) scanf("%d%d%d",&x,&y,&z)
#define RD4(x,y,z,w) scanf("%d%d%d%d",&x,&y,&z,&w)
#define All(vec) vec.begin(),vec.end()
#define MP make_pair
#define PII pair<int,int>
#define PQ priority_queue
#define cmax(x,y) x = max(x,y)
#define cmin(x,y) x = min(x,y)
#define Clear(x) memset(x,0,sizeof(x))
#define lson rt<<1
#define rson rt<<1|1
#define SZ(x) x.size()

/*

#pragma comment(linker, "/STACK:1024000000,1024000000")

int ssize = 256 << 20; // 256MB
char *ppp = (char*)malloc(ssize) + ssize;
__asm__("movl %0, %%esp\n" :: "r"(ppp) );

*/

char IN;
bool NEG;
inline void Int(int &x){
    NEG = 0;
    while(!isdigit(IN=getchar()))
        if(IN=='-')NEG = 1;
    x = IN-'0';
    while(isdigit(IN=getchar()))
        x = x*10+IN-'0';
    if(NEG)x = -x;
}
inline void LL(ll &x){
    NEG = 0;
    while(!isdigit(IN=getchar()))
        if(IN=='-')NEG = 1;
    x = IN-'0';
    while(isdigit(IN=getchar()))
        x = x*10+IN-'0';
    if(NEG)x = -x;
}

/******** program ********************/

const int MAXN = 152;

string sa[MAXN],sb[MAXN];
string ta[MAXN];

int main(){

#ifndef ONLINE_JUDGE
    freopen("sum.in","r",stdin);
    freopen("sum.out","w",stdout);
#endif

    int ncase,cnt = 0;
    RD(ncase);
    while(ncase--){
        printf("Case #%d: ",++cnt);
        int n,len;
        RD2(n,len);
        rep(i,n)
            cin>>sa[i];
        rep(i,n)
            cin>>sb[i];
        int ans = 1001;
        sort( sb,sb+n );
        rep(i,n)
            ta[i] = sa[i];

        rep(i,n){
            rep(j,n){
                int tot = 0;
                rep(y,len)
                    tot += (sa[i][y]!=sb[j][y]);
                rep(x,n){
                    rep(y,len){
                        if(sa[i][y]!=sb[j][y]){
                            if(sa[x][y]=='0')
                                ta[x][y] = '1';
                            else
                                ta[x][y] = '0';
                        }else
                            ta[x][y] = sa[x][y];
                    }
                }

                sort(ta,ta+n);
                rep(x,n)
                    if(ta[x]!=sb[x]){
                        tot = -1;
                        break;
                    }
                if(tot!=-1&&tot<ans)
                    ans = tot;
            }
        }

        if(ans==1001)
            puts("NOT POSSIBLE");
        else
            cout<<ans<<endl;
    }

    return 0;
}
