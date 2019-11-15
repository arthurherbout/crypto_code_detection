#include<cmath>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<iostream>
#include<cstdio>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

int main()
{
    int cases;
    cin >> cases;
    REP(cc,cases)
    {
        ll N, M, A;
        cin >> N >> M >> A;
        printf("Case #%d: ", cc+1);
        try
        {
            for(ll a = 0; a <= N; ++a)
                for(ll b = 0; b <= M; ++b)
                {                
                    for(ll y = 0; y <= M; ++y)
                        for(ll x = 0; x <= N; ++x)
                        {
                            double a1 = hypot(b, a);
                            double b1 = hypot(y, x-a);
                            double c1 = hypot(y-b, x);
                            double area = sqrt((a1+(b1+c1))*(c1-(a1-b1))*(c1+(a1-b1))*(a1+(b1-c1))) / 2;
                            if(fabs(area-A) < 0.01)
                            {
                                printf("%Ld 0 0 %Ld %Ld %Ld\n", a, b, x, y);
                                throw 1;
                            }                        
                        }
                }
            printf("IMPOSSIBLE\n");
        }
        catch(int x)
        {
        }
    }
     
}

