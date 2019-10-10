#include<bits/stdc++.h>
#define assn(n,a,b) assert(n<=b && n>=a)
using namespace std;
#define pb push_back
#define mp make_pair
#define clr(x) x.clear()
#define sz(x) ((int)(x).size())
#define F first
#define S second
#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,b) for(i=0;i<b;i++)
#define rep1(i,b) for(i=1;i<=b;i++)
#define pdn(n) printf("%d\n",n)
#define sl(n) scanf("%lld",&n)
#define sd(n) scanf("%d",&n)
#define pn printf("\n")
typedef pair<int,int> PII;
typedef vector<PII> VPII;
typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long LL;
#define MOD 1000000007ll
LL mpow(LL a, LL n) 
{LL ret=1;LL b=a;while(n) {if(n&1) 
    ret=(ret*b)%MOD;b=(b*b)%MOD;n>>=1;}
return (LL)ret;}
LL b,n,ar[1009];
LL foo(LL tim){
    LL ret=0;
    for(int i=0; i<b; i++)
        ret += (tim/ar[i]) + (tim%ar[i]>0);
    return ret;
}
int main()
{
    int t,p;
    sd(t);
    p=t;
    while(t--){
        int r,c,n,mat[17][17],ans=INT_MAX;
        cin >> r >> c >> n;
        for(int i=0; i<(1<<(r*c)); i++){
            if(__builtin_popcount(i)!=n)continue;
            memset(mat,0,sizeof(mat));
            for(int j=0; j<r*c; j++)
                if(i&(1<<j))mat[j/c][j%c]=1;
            int cnt=0;
            for(int ii=0; ii<r-1; ii++)
                for(int j=0; j<c; j++)
                    if(mat[ii][j]+mat[ii+1][j]==2)cnt++;
            for(int j=0; j<c-1; j++)
                for(int ii=0; ii<r; ii++)
                    if(mat[ii][j]+mat[ii][j+1]==2)cnt++;
            ans=min(ans,cnt);
        }
        printf("Case #%d: %d\n",p-t,ans);
    }
    return 0;
}
