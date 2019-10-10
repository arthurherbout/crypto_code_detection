#include<iostream>
#include<cstring>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef vector<int> VI;
typedef long long Long;

const int LIMIT = 1000*1000;

Long fence;
int n, boards[100];
int sol[LIMIT+1];

void solve() {
    sol[0] = 0;
    sort(boards, boards+n);
    Long ret = -1;
    FOR(i,0,LIMIT+1) {
        if(i == 0)
            sol[i] = 0;
        else {
            sol[i] = -1;
            REP(j,n) {
                int left = i - boards[j];
                if(left >= 0 && sol[left] != -1) {
                    if(sol[i] == -1)
                        sol[i] = sol[left] + 1;
                    else
                        sol[i] = min(sol[i], sol[left] + 1);
                }
            }
        }
        Long left = fence - i;
        if(left % boards[n-1] == 0 && sol[i] != -1) {
            Long cand = left / boards[n-1] + sol[i];
            if(ret == -1 || ret > cand) {
                /*
                printf("Can make %Ld = %Ld + %d by constructing up to %d\n",
                        cand, left / boards[n-1], sol[i], i);
                        */
                ret = cand;
            }
        }
    }
    if(ret != -1)
        printf("%Ld\n", ret);
    else
        printf("IMPOSSIBLE\n");
}

int main()
{
    int cases;
    scanf("%d", &cases);
    for(int cc = 0; cc < cases; ++cc)
    {
        scanf("%Ld %d", &fence, &n);
        REP(i,n) scanf("%d", &boards[i]);
        printf("Case #%d: ", cc+1);
        solve();
    }
}

