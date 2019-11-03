#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h> 

using namespace std;
long long mod=1e9+7;
long long INF=1e12;
double EPS = 1e-12;
typedef long long int lld;
// double tick(){static clock_t oldt;clock_t newt=clock();double diff=1.0*(newt-oldt)/CLOCKS_PER_SEC;oldt = newt;return diff;}
#define rep(i,a,n) for(long long int i = (a); i <= (n); ++i)
#define repI(i,a,n) for(int i = (a); i <= (n); ++i)
#define repD(i,a,n) for(long long int i = (a); i >= (n); --i)
#define repDI(i,a,n) for(int i = (a); i >= (n); --i)
#define pb push_back
#define mp make_pair
#define ff first
#define ss second
#define sc(a) scanf("%lld",&a)
#define sc2(a,b) scanf("%lld%lld",&a,&b)
#define sc3(a,b,c) scanf("%lld%lld%lld",&a,&b,&c)
#define scd(a) scanf("%d",&a)
#define scd2(a,b) scanf("%d%d",&a,&b)
#define scd3(a,b,c) scanf("%d%d%d",&a,&b,&c)
#define scf(a) scanf("%lf",&a)
#define scf2(a,b) scanf("%lf%lf",&a,&b)
#define scf3(a,b,c) scanf("%lf%lf%lf",&a,&b,&c)
#define prL(a) printf("%lld\n",a)
#define prS(a) printf("%lld ",a)
#define prdL(a) printf("%d\n",a)
#define prdS(a) printf("%d ",a)
#define all(c) c.begin(), c.end()
#define sz(a) ((int)a.size())
#define Error(x) cout<< #x << " = " << (x) <<endl;
#define Error2(a,b) cout<<"( "<<#a<<" , "<<#b<<" ) = ( "<<(a)<<" , "<<(b)<<" )\n";
#define Error3(a,b,c) cout<<"("<<#a<<" , "<<#b<<" , "<<#c<<" ) = ( "<<(a)<<" , "<<(b)<<" , "<<(c)<<")\n";
#define Error4(a,b,c,d) cout<<"("<<#a<<" , "<<#b<<" , "<<#c<<" , "<<#d<<" ) = ( "<<(a)<<" , "<<(b)<<" , "<<(c)<<" , "<<(d)<<")\n";
#define errop(a) cout<<#a<<" = ( "<<((a).x)<<" , "<<((a).y)<<" )\n";
#define mset(a, v) memset(a, v, sizeof(a))
#define popcount __builtin_popcountll

typedef pair<lld,lld> PA;

lld powP(lld a,lld b){
    if(b==0) return 1%mod;
    lld k;
    k=powP(a,b/2);
    k=k*k%mod;
    if(b%2==0) return k;
    else return a*k%mod;
}

bool bitSet(lld n,lld i){
    if((n&(1LL<<i))!=0) return true;
    else return false;
}

lld findGcd(lld a,lld b){
    if(a==0) return b;
    else return findGcd(b%a,a);
}

#define lim 700005
#define lim2 200003
#define qqqqqq 210
// inline lld sqr(lld x) { return x * x; }
// unordered_map<lld,lld>::iterator it;
// std::ios::sync_with_stdio(false);
// priority_queue<PA> Q;
// lld dp[1<<18];
// vector<lld> V[lim];
string S[lim];
lld hMe,aMe,hOp,aOp,org,turn,alpha;
lld b,d,A[lim],B[lim],dp[103][153][103][103],Resume[103][153][103][103];
// unordered_map<lld,lld> M,X;
// map<lld,lld> M;

lld compute(lld hMe,lld aMe,lld hOp,lld aOp){
    // Error4(hMe,aMe,hOp,aOp);
    // turn++;
    // if(turn>=200) return 1;
    if(hOp<=0) return 0;
    // printf("aaya\n");
    if(hMe<=0 || aMe>=150) return INF;
    if(Resume[hMe][aMe][hOp][aOp]==alpha) return dp[hMe][aMe][hOp][aOp];
    Resume[hMe][aMe][hOp][aOp] = alpha;
    dp[hMe][aMe][hOp][aOp] = INF;
    // ans = compute(hMe-aOp,aMe,hOp-aMe,aOp);
    lld ans;
    ans = min(compute(hMe-aOp,aMe,hOp-aMe,aOp),compute(hMe-aOp,aMe+b,hOp,aOp));
    // Error4(hMe,aMe,hOp,aOp);
    // Error3(a1,a2,ans);
    ans = min(ans,min(compute(org-aOp,aMe,hOp,aOp),compute(hMe-aOp,aMe,hOp,max(0LL,aOp-d))));
    // Error4(hMe,aMe,hOp,aOp);
    // Error4(a1,a2,a3,a4);
    // Error2(min(a1,a2),ans);
    return dp[hMe][aMe][hOp][aOp] = ans+1;
}

int main(){
    lld T,i,n,m,p,j,l,e,r,c,k,q,a,w,x,y,v,z,t,curr,prev,sum,ans,pos,val,countA,secondMin,indicator;
    sc(T);
    rep(testC,1,T){
        printf("Case #%lld: ",testC);
        sc3(hMe,aMe,hOp);
        sc3(aOp,b,d);
        org = hMe;
        // turn=0;
        alpha = testC;
        t = compute(hMe,aMe,hOp,aOp);
        // Error(turn);
        if(t>=INF) printf("IMPOSSIBLE\n");
        else prL(t);
        // Error2(dp[100][150][100][100],Resume[100][150][100][100]);
        // cout<<tick()<<endl;
    }
    return 0;
}