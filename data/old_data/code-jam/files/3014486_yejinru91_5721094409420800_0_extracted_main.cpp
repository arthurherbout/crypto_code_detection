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

const int MAXN = 1005;

int dpl[MAXN],dpr[MAXN];
int a[MAXN],b[MAXN];
int n;

int bubble_r(int l){
    if(l>n)return 0;
    int tot = 0;
    for(int i=n;i>=l;i--)
        b[++tot] = a[i];
    int ans = 0;
    for(int i=tot;i;i--){
        for(int j=1;j<i;j++)
        if(b[j]>b[j+1]){
            ans ++;
            swap(b[j],b[j+1]);
        }
    }
    return ans;
}

int bubble_l(int r){
    if(r==0)
        return 0;
    rep1(i,r)
        b[i] = a[i];

    int ans = 0;
    for(int i=r;i;i--){
        for(int j=1;j<i;j++)
        if(b[j]>b[j+1]){
            ans ++;
            swap(b[j],b[j+1]);
        }
    }
    return ans;
}

int main(){

#ifndef ONLINE_JUDGE
    freopen("sum.in","r",stdin);
    freopen("sum.out","w",stdout);
#endif

    int ncase , cnt = 0;
    RD(ncase);
    while(ncase--){
        printf("Case #%d: ",++cnt);
        RD(n);
        rep1(i,n)
            RD(a[i]);
        int ans = n*n;
        for(int mid=0;mid<=n;mid++){
            int l = bubble_l(mid);
            int r = bubble_r(mid+1);
            ans = min(ans,l+r);
        }
        cout<<ans<<endl;
    }

    return 0;
}
