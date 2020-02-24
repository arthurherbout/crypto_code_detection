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
typedef vector<VI> VVI;
typedef long long ll;

string expr; int k;
vector<string> terms;
vector<string> dict;
const int M = 10009;

int go(int choose, VI& cnt)
{
    if(choose == 0)
    {
        int ret = 0;
        REP(i,terms.sz)
        {
            int t = 1;
            REP(j,terms[i].sz)
                t = (t * cnt[terms[i][j] - 'a']) % M;
            (ret += t) %= M;
        }
        return ret;
    }
    int ret = 0;
    REP(i,dict.sz)
    {
        REP(j,dict[i].sz)
            cnt[dict[i][j] - 'a']++;
        (ret += go(choose-1, cnt)) %= M;
        REP(j,dict[i].sz)
            cnt[dict[i][j] - 'a']--;
    }
    return ret;
}

int main()
{
    int cases;
    cin >> cases;
    for(int cc = 0; cc < cases; ++cc)
    {
        terms.clear(); dict.clear();
        cin >> expr >> k;
        replace(all(expr), '+', ' ');
        istringstream inp(expr);
        string tok;
        while(inp >> tok) terms.pb(tok);
        int n;
        cin >> n;
        dict.resize(n);
        REP(i,n) cin >> dict[i];
        printf("Case #%d:", cc+1);
        VI cnt(26, 0);
        FOR(i,1,k+1) printf(" %d", go(i, cnt));
        printf("\n");
    }
}

