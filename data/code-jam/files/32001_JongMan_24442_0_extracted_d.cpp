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

int k;
string s;
string p;

int main()
{
    int cases;
    cin >> cases;

    REP(cc,cases)
    {
        cin >> k >> s;
        p = s;
        int ret = s.sz;
        vector<int> perm(k);
        REP(i,k) perm[i] = i;
        do
        {
            for(int a = 0; a < p.sz; a += k)
                REP(b,k)
                    p[a+b] = s[a+perm[b]];
            int cand = 1;
            FOR(i,1,p.sz) if(p[i] != p[i-1]) ++cand;
            ret <?= cand;            
        } while(next_permutation(all(perm)));
        printf("Case #%d: %d\n", cc+1, ret);
    }
}

