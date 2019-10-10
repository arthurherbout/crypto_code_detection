#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <queue>
#include <set>
#include <map>
#include <vector>
#include <complex>

#define mp make_pair
#define pb push_back
#define sqr(x) ((x)*(x))
#define foreach(it,c) for(typeof((c).begin()) it = (c).begin(); it != (c).end(); ++it)

using namespace std;

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef vector<string> VS;
typedef pair<int,int> PII;
typedef long long LL;
typedef complex<double> Point;

template<typename T> inline int size(const T &a) { return a.size(); }
template<typename T> inline bool operator<(const int &a,const vector<T> &b) { return a<b.size(); }

LL mod = 1000000007LL;

LL N;
int B;
vector<int> arr;
map<int,int> mm[71];
map<int,int> mm0[71];

struct mmemo
{
    int a,b;
    bool operator<(const mmemo &aa) const
    {
        if(a != aa.a) return a<aa.a;
        if(b != aa.b) return b<aa.b;
    }
};

map<mmemo,LL> memo;

void precalc(int B)
{
    for(int i=0;i<71;i++) 
    {
        mm[i].clear();
        mm0[i].clear();
    }

    for(int i=1;i<(1<<B);i++)
    {
        int bcnt = 0;
        int sum = 0;
        for(int j=0;j<B;j++) if(i & (1<<j)) 
        {
            bcnt++;
            sum += j;
        }
        if(i & 1)
        {
            mm0[bcnt][sum]++;
        }
        else
        {
            mm[bcnt][sum]++;
        }
    }
}

LL calc(int tgt,int cnt)
{
    mmemo mmmm;
    mmmm.a = tgt;
    mmmm.b = cnt;

    if(tgt == 0)
    {
        if(cnt) return 0;
        return 1;
    }

    if(cnt==0)
        return 0;

    if(memo.find(mmmm) != memo.end()) return memo[mmmm];

    LL ret = 0;

    if(mm[cnt].size() == 0) return 0;

    int maxx = min(mm[cnt].rbegin()->first, tgt);
    int num = tgt % B;
    for(int j=num;j<=maxx;j+=B)
    {
        if(j != 0)
        {
            int numcnt = mm[cnt][j];
            if(numcnt)
            {
                for(int i=0;i<=cnt;i++)
                {
                    ret = (ret + calc((tgt - j) / B, i) * numcnt) % mod;
                    numcnt *= cnt-i;
                }
            }
        }

        int numcnt = mm0[cnt][j];
        if(numcnt)
        {
            for(int i=1;i<=cnt;i++)
            {
                ret = (ret + calc((tgt - j) / B, i) * numcnt) % mod;
                numcnt *= cnt - i;
            }
        }
    }
    return memo[mmmm] = ret;
}

void process(void)
{
    cin >> N >> B;
    precalc(B);
    memo.clear();
    LL ret = 0;
    for(int i=1;i<=B;i++)
    {
        ret = (ret + calc(N, i)) % mod;
    }
    cout << ret << endl;
}

int main(void)
{
    int T;
    scanf("%d",&T);
    for(int i=1;i<=T;i++)
    {
        printf("Case #%d: ",i);
        process();
    }
	return 0;
}

