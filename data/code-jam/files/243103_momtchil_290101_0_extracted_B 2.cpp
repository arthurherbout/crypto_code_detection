#include <vector>
#include <cstring>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>


#define SZ size()
#define PB push_back
#define ALL(v) v.begin(),v.end()
#define MP make_pair
#define x first
#define y second

#define LL long long
#define UI unsigned int
#define ULL unsigned long long
#define PI pair<int,int>
#define PD pair<double,double>
#define PLL pair<LL,LL>
#define PULL pair<ULL,ULL>
#define VI vector<int>
#define VD vector<double>
#define VS vector<string>
#define SI set<int>
#define PQ priority_queue
#define VVI vector<vector<int> >
#define IT iterator

#define ABS(x) (((x)>0)?(x):(-(x)))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))
#define sign(a) ((a)>0)-((a)<0)
#define sqr(a) ((a)*(a))
#define F(i,n) for (int i=0;i<n;i++)
#define Repi(n) for (int i=0; i<n; i++)
#define Repj(n) for (int j=0; j<n; j++)
#define Repk(n) for (int k=0; k<n; k++)
#define F(i,n) for (int i=0;i<n;i++)

#define INF 2000000000
#define EPS 1e-6

#define Time ((double)clock()/CLOCKS_PER_SEC)
#define pause system("pause")

using namespace std;

int T;

int cnt[128];
int K,N;
string s[1000];

int mod=10009;
int f[25];
int sum[30];
string p;

int called;
inline int calc()
{
    int res=0;
    
    int cur=-1;
    Repi(p.SZ)
     {
            char let=p[i];
            if (let=='+')
             {
                    res += cur;
                   // cout<<"                 res "<<res<<" += "<<cur<<"\n";
                    res%=mod;
                    cur=-1;
             }
            else
             {
                    if (cur==-1)
                        cur=cnt[let];
                    else cur=cur*cnt[let] , cur %= mod;
                    //cout<<"                         cur = "<<cur<<" ("<<let<<" = "<<cnt[let]<<")\n";
             }
     }
    res += cur;
    res%=mod;
  //  cout<<"                         res = "<<res<<"\n";
  called++;
    return res;
}

vector<string> c;
int inc[101][128];
string ss[101];


void choose(int taken)
{
    if (taken > K) return;
    
    if (taken)
     {
            int add=calc();
            sum[taken]+=add%mod;
            
            sum[taken]%=mod;
     }

    if (taken==K) return;
    
    F(id,N)
     {
//       Repi(s[id].SZ)        cnt[s[id][i]]++;
//       for (char c='a'; c<='z'; c++)
    Repj(ss[id].SZ)
        cnt[ss[id][j]]+=inc[id][ss[id][j]];
        
       choose(taken+1);

    Repj(ss[id].SZ)
        cnt[ss[id][j]]-=inc[id][ss[id][j]];
//       Repi(s[id].SZ)        cnt[s[id][i]]--;
     }
}

int main()
{
    scanf("%d",&T);
    f[0]=1;
    for (int i=1;i<=20;i++)
     f[i]=f[i-1]*i%mod;
    F(xx,T)
     {
                cin>>p>>K;
                cin>>N;
                memset(inc,0,sizeof(inc));
                Repi(N)
                 {
                   cin>>s[i];
                   ss[i]="";
                   Repj(s[i].SZ)
                    inc[i][s[i][j]]++;
                   for (char c='a'; c<='z'; c++)
                    if (inc[i][c])
                     ss[i]+=c;
                 }
                 called=0;
                memset(sum,0,sizeof(sum));
                choose(0);
                cerr<<called<<"\n";
                cout<<"Case #"<<xx+1<<":";
                cerr<<Time<<"\n";
                Repi(K)
                 cout<<" "<<sum[i+1];
                cout<<"\n";
                cerr<<xx<<"\n";
     }
    return 0;
}
