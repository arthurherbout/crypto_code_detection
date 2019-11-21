#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<vector>
#include<algorithm>
#include<bitset>
#include<list>
#include<set>
#include<unordered_set>
#include<map>
#include<stack>
#include<queue>
#include<cmath>
#include<string>
#include<cstring>
#include<sstream>
#include<climits>

using namespace std;

typedef long long ll;
typedef vector<int> vi;
typedef pair<int, int> ii;
typedef vector<ii> vii;
typedef set<int> si;
typedef map<string, int> msi;

#define S(x) scanf("%d",&x)
#define SL(x) scanf("%lld",&x)
#define SD(x) scanf("%lf",&x)
#define pb(x) push_back(x)
#define mp make_pair
#define F(i, a, b) for (int i = int(a); i < int(b); i++)
#define forit(it, a) for (it = (a).begin(); it != (a).end(); it++)
#define M(x, i) memset(x,i,sizeof(x))

/* -------------------CODE GOES HERE---------------------- */

int graph[101][101];

int main() {
    int T; S(T);
    int  tst = 1,temp;

    int N,Q,U,V;
    vi E,S,D;
    vector<double> t;

    while(T--){

        S(N); S(Q);

        E.clear(); S.clear();
        D.clear(); t.clear();
        D.pb(0);
        t.clear();

        F(i,0,N){
            t.pb(19999999999999.99);
        }

        F(i,0,N){
            S(temp);
            E.pb(temp);
            S(temp);
            S.pb(temp);
        }

        F(i,0,N) F(j,0,N) S(graph[i][j]);
        S(U); S(V);

        F(i,0,N-1){
            D.pb(D[i] + graph[i][i+1]);
        }

        t[0] = 0.0;

        F(i,0,N) {
            F(j,i+1,N){
                if(D[j] - D[i] - E[i] > 0.0001){
                    break;
                }
                t[j] = min(t[j],t[i] + ((D[j]-D[i])*1.0)/(S[i]*1.0));
            }
        }

        printf("Case #%d: %0.9lf\n", tst++, t[N-1]);
    }
}
