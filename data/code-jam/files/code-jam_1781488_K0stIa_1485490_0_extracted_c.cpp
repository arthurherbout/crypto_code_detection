#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <iostream>
#include <cmath>
#include <sstream>
#include <map>
#include <set>
#include <numeric>
#include <memory.h>
#include <cstdio>

using namespace std;

#define pb push_back
#define INF 101111111
#define FOR(i,a,b) for (int _n(b), i(a); i < _n; i++)
#define rep(i,n) FOR(i,0,n)
#define ford(i,a,b) for(int i=(a),_b=(b);i>=_b;--i)
#define CL(a,v) memset((a),(v),sizeof(a))
#define mp make_pair
#define X first
#define Y second
#define all(c) (c).begin(), (c).end()
#define SORT(c) sort(all(c))

typedef long long ll;
typedef vector<int> VI;
typedef pair<int,int> pii;
typedef unsigned long long ull;

ull a[111], b[111];
int A[111], B[111];
ull dp[111][111];
int N,M;

ull solve_hard()
{
    CL(dp,0);

    rep(i,N) rep(j,M)
    {
        const int ni = i + 1, nj = j + 1;

        dp[ni][nj] = max(dp[ni][nj],dp[i][j] + (A[i]==B[j] ? min(a[i],b[j]) : (ull)0));
        dp[ni][j] = max(dp[ni][j], dp[i][j]);
        dp[i][nj] = max(dp[i][nj], dp[i][j]);
    }

    ull ans = 0;
    rep(i,N) ans = max(ans, dp[i][M]);
    rep(i,M) ans = max(ans, dp[N][i]);

    return ans;
}

ull solve_1(ull rem, int id, int f, int e)
{
    ull ans = 0;

    FOR(i,f,e) if(B[i] == id)
    {
        ans += min(rem, b[i]);
        rem -= min(rem, b[i]);
    }

    return ans;
}

ull solve_2(ull *aa, int *id, int f, int e)
{
    if(id[0] == id[1])
    {
        return solve_1(aa[0]+aa[1], id[0], f, e);
    }

    ull ans = 0;

    rep(ni,M)
    {
        ull tmp = 0;
        ull c[2] = {aa[0], aa[1]};

        rep(i,ni) if(B[i]==id[0])
        {
            tmp += min(c[0],b[i]);
            c[0] -= min(c[0],b[i]);
        }

        FOR(i,ni,M) if(B[i]==id[1])
        {
            tmp += min(c[1],b[i]);
            c[1] -= min(c[1],b[i]);
        }

        ans = max(ans, tmp);
    }

    return ans;
}

ull solve_3(ull *aa, int *id, int f, int e)
{
    if(id[0] == id[1] && id[0] == id[2])
    {
        return solve_1(aa[0]+aa[1]+aa[2], id[0], 0, M);
    }

    if(id[0]==id[1])
    {
        ull na[2] = {aa[0]+aa[1], aa[2]};
        int nid[2] = {id[0], id[2]};

        return solve_2(na,nid,0,M);
    }

    if(id[2]==id[1])
    {
        ull na[2] = {aa[0],aa[1]+aa[2]};
        int nid[2] = {id[0], id[2]};

        return solve_2(na,nid,0,M);
    }

    ull ans = 0;

    rep(ni,M) FOR(nj,ni+1,M-1)
    {
        ull tmp = 0;
        ull aa[3] = {a[0], a[1], a[2]};

        rep(i,ni) if(B[i]==A[0])
        {
            tmp += min(aa[0],b[i]);
            aa[0] -= min(aa[0],b[i]);
        }

        FOR(i,ni,nj) if(B[i]==A[1])
        {
            tmp += min(aa[1],b[i]);
            aa[1] -= min(aa[1],b[i]);
        }

        FOR(i,nj,M) if(B[i]==A[2])
        {
            tmp += min(aa[2],b[i]);
            aa[2] -= min(aa[2],b[i]);
        }

        ans = max(ans, tmp);
    }

    return ans;
}

ull solve_easy()
{
    if(N==1)
    {
        return solve_1(a[0],A[0],0,M);
    }

    if(N==2)
    {
        return solve_2(a,A,0,M);
    }

    ull ans = 0;

    if(N==3)
    {
        FOR(s,1,1<<3)
        {
            const int sz = __builtin_popcount(s);

            if(sz==1)
            {
                int id = -1;
                for(; id < 3; ++id) if((1<<id)&s) break;

                ans = max(ans, solve_1(a[id],A[id],0,M));
            }

            if(sz==2)
            {
                ull aa[2] = {0,0};
                int id[2] = {-1,-1};
                int cnt = 0;

                rep(i,3) if((1<<i)&s)
                {
                    id[cnt] = A[i];
                    aa[cnt] = a[i];
                    cnt ++;
                }

                ans = max(ans,solve_2(aa,id,0,M));
            }

            if(sz==3)
            {
                ans = max(ans, solve_3(a,A,0,M));
            }
        }
    }

    return ans;
}

int main()
{
	#ifndef ONLINE_JUDGE
        //freopen("input.txt","r",stdin);
        freopen("C-small-attempt0.in","r",stdin);
        freopen("C-small-attempt0.txt","w",stdout);
	#endif

    int T;
    cin >> T;


    FOR(cs,1,T+1)
    {
        cin >> N >> M;
        rep(i,N) cin >> a[i] >> A[i];
        rep(i,M) cin >> b[i] >> B[i];

        //ull ans = solve_hard();
        ull ans = solve_easy();

        printf("Case #%d: ",cs);
        cout << ans << endl;
    }

	return 0;
}
