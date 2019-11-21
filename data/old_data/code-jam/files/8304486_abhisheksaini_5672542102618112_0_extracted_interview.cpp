#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h> 
 
using namespace std;
long long mod=1e9+7;
long long INF=1e7;
double EPS = 1e-12;
typedef long long int lld;
double tick(){static clock_t oldt;clock_t newt=clock();double diff=1.0*(newt-oldt)/CLOCKS_PER_SEC;oldt = newt;return diff;}
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
 
#define lim 100010
#define lim2 200003
#define qqqqqq 210
// inline lld sqr(lld x) { return x * x; }
// unordered_map<lld,lld>::iterator it;
// std::ios::sync_with_stdio(false);
// priority_queue<PA> Q;
// lld dp[1<<18];
lld V[7][lim],P[7][lim];
// int X[lim],Y[lim],C[lim];
// string S[lim];
lld A[lim],B[lim],C[lim],L,dp[7][lim];
// map<lld,lld> M;

void dfs(lld x,lld dis){
    if(dp[L][x]==0) dis++;
    // if(L==1 && x==1) Error2(x,P[L][x]);
    dp[L][x] += dis;
    if(P[L][x] != x){
        // if(L==2 && P[L][x]==12){
            // Error3(x,P[L][x],dis);
        // }
        dfs(P[L][x],dis);

    }
}

lld convert(lld x){
    lld countA = 0;
    rep(i,1,L) A[i] = 0;
    while(x>0){
        if(x%10>L) return -1;
        A[x%10]++;
        countA++;
        x /= 10;
    }
    if(countA>L) return -1;
    lld ans = 0;
    rep(i,1,L) ans = ans*10 + A[i];
    return ans;
}


int main(){
    lld T,i,m,n,p,j,l,e,r,b,c,k,q,a,d,w,x,y,v,z,t,curr,prev,sum,ans,pos,val,countA,indicator;
    lld maxLimit = 1e5;
    rep(i,1,5){
        L = i;
        rep(j,1,maxLimit) P[i][j] = V[i][j] = dp[i][j] = 0;
        rep(j,1,maxLimit){
            k = convert(j);
            if(k==-1){
                P[i][j] = -1;
                continue;
            }
            if(k==j){
                P[i][j] = k;
                continue;
            }
            V[i][k] = (j);
            P[i][j] = (k);
        }
        rep(j,1,maxLimit) if(V[i][j]==0 && P[i][j]!=-1) dfs(j,0LL);
    }
    sc(T);
    rep(tC,1,T){
        printf("Case #%lld: ",tC );
        string S;
        cin>>S;
        L = S.size();
        p = stoll(S);
        // Error2(L,p);
        prL(dp[L][p]);
    }
    return 0;
}