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
typedef long double LL;
#define MOD 1000000007ll
LL globx,globy;
bool cmp(PII a, PII b){
    return a.F*b.S>a.S*b.F;
}
int main()
{
    int t,p;
    sd(t);
    p=t;
    while(t--){
        int n,di,hi,mi,cnt=0;
        VPII ar;
        ar.resize(2);
        sd(n);
        for(int i=0; i<n; i++){
            sd(di),sd(hi),sd(mi);
            for(int j=0; j<hi; j++)
                ar[cnt].F=di,ar[cnt].S=mi+j,cnt++;
        }
        if(cnt==1){
            printf("Case #%d: 0\n",p-t);
            continue;
        }
        printf("Case #%d: ",p-t);
        sort(ar.begin(),ar.end());
        double x1=ar[0].F,y1=ar[0].S,x2=ar[1].F,y2=ar[1].S;
        //        printf("%lf %lf\n",(2.0-x2/360.0)*y2,(1-x1/360.0)*y1);
        if((2.0-x2/360.0)*y2<(1-x1/360.0)*y1+1e-10
                ||
                ( (1.0-x2/360.0)*y2>(2-x1/360.0)*y1))printf("1\n");
        else printf("0\n");
    }
    return 0;
}
