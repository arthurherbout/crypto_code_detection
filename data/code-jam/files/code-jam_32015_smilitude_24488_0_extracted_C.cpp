#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<cctype>

#include<cmath>
#include<iostream>
#include<fstream>
#include<numeric>

#include<string>
#include<vector>
#include<queue>
#include<map>
#include<algorithm>
#include<set>
#include<sstream>
#include<stack>
#include<list>
#include<iterator>
using namespace std;

#define REP(i,n) for(__typeof(n) i=0; i<(n); i++)
#define FOR(i,a,b) for(__typeof(b) i=(a); i<=(b); i++)
#define CLEAR(t) memset((t), 0, sizeof(t))

#define sz size()
#define pb push_back
#define pf push_front

#define VI vector<int>
#define VS vector<string>
#define LL long long

vector<LL> elem;
vector<LL> dp;
vector<LL> A;

#define modval 1000000007


int main() {
    
    int N;
    LL n,m,X,Y,Z;
    
    freopen("c1.in","r",stdin);
    freopen("c1.out","w",stdout);
    
    cin >> N;
    REP(ncase, N) {
        cin >> n >> m >> X >> Y >> Z;
        A.resize( m );
        elem.resize( n );
        REP(i,m) cin>>A[i];
        REP(i,n) {
            elem[i] =  A[ i % m ];
            A[ i % m ] = (X * A[i % m] + Y * (i + 1)) % Z;
        }
        
        dp = vector<LL> (n,1);
        REP(i,n) {
            REP(j,i) {
                if( elem[j] < elem[i] ) { dp[i] += dp[j]; dp[i] %= modval; }
            }
        }        
        
        LL ans = 0;
        REP(i,n) { ans+=dp[i]; ans %= modval; }
        
        printf("Case #%d: %lld\n",ncase+1,ans);
    }
    

    //system("pause");
    
    return 0;
}
