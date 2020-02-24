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
 
#define lim 500010
#define lim2 200003
#define qqqqqq 210
// inline lld sqr(lld x) { return x * x; }
// unordered_map<lld,lld>::iterator it;
// std::ios::sync_with_stdio(false);
// priority_queue<PA> Q;
// lld dp[1<<18];
// vector<lld> V[lim];
// int X[lim],Y[lim],C[lim];
// string S[lim];
lld A[lim],Ra[lim],Ca[lim],dp[203][203],P[203][203],Dis[203][203];
// map<lld,lld> M;
lld dx[] = {-1,1,0,0},dy[] = {0,0,-1,1};
lld R,C,N,D;

bool isVal(lld x,lld y){
    if(x<1 || x>R || y<1 || y>C) return false;
    else return true;
}

void bfs(lld x,lld y,lld dis){
    lld i,j,a,b;
    PA k;
    queue<PA> Q;
    Q.push({x,y});
    P[x][y] = 1;
    Dis[x][y] = dis;
    dp[x][y] = min(dp[x][y],Dis[x][y]);
    while(!Q.empty()){
        k=Q.front();
        Q.pop();
        rep(i,0,3){
            a = k.ff + dx[i];
            b = k.ss + dy[i];
            if(isVal(a,b) && P[a][b]==0){
                P[a][b] = 1;
                Dis[a][b] = Dis[k.ff][k.ss] + D;
                dp[a][b] = min(dp[a][b],Dis[a][b]);
                Q.push({a,b});
            }
        }
    }
}

lld check(){
    lld a,b;
    rep(i,1,R) rep(j,1,C) rep(k,0,3){
        a = i + dx[k];
        b = j + dy[k];
        if(isVal(a,b) && abs(dp[a][b]-dp[i][j])>D) return -1;
    }
    return 1;
}
lld findSum(){
    lld a = 0;
    rep(i,1,R) rep(j,1,C){
        a = (a + dp[i][j]%mod)%mod;
    }
    return a;
}
int main(){
    lld T,i,m,n,p,j,l,e,r,b,c,k,q,a,d,w,x,y,v,z,t,curr,prev,sum,ans,pos,val,countA,indicator;
    sc(T);
    rep(tC,1,T){
        printf("Case #%lld: ",tC );
        sc2(R,C);
        sc2(N,D);
        rep(i,1,R) rep(j,1,C){
            dp[i][j] = LLONG_MAX;
            // P[i][j] = 0;
        }
        rep(i,1,N){
            sc3(Ra[i],Ca[i],A[i]);
            // P[R[i]][C[i]] = 1;
        }
        rep(zoi,1,N){
            rep(i,1,R) rep(j,1,C){
                P[i][j] = 0;
                Dis[i][j] = 0;
            }
            rep(i,1,N){
                // sc3(R[i],C[i],A[i]);
                P[Ra[i]][Ca[i]] = 1;
            }
            bfs(Ra[zoi],Ca[zoi],A[zoi]);
        }
        // rep(i,1,R) rep(j,1,C) Error3(i,j,dp[i][j]);
        z = check();
        if(z==-1) printf("IMPOSSIBLE\n");
        else prL(findSum());
    }
    return 0;
}