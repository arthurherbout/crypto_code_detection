#include <iostream>
#include <fstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <cmath>
#include <cassert>
#include <ctime>
#include <algorithm>
#include <queue>
#include <iterator>
#include <cstdio>
#include <cstdlib>
#include <memory.h>
#include <stack>
#define mp make_pair
#define pb push_back   
#define setval(a,v) memset(a,v,sizeof(a))
using namespace std;

typedef long long ll;
typedef long double ld;

const string TASKNAME = "C";
const string TASKMOD = "small";
const string INFILE = TASKNAME+"-"+TASKMOD+".in";
const string OUTFILE = TASKNAME+"-"+TASKMOD+".out";



void PreCalc(){
}

int c[10];
int p[10];
int v[10];
int n;

struct rest{
    ld l,r;
    int to;
    rest(ld l,ld r,int to):l(l),r(r),to(to){
    }
};

vector<rest> g[10];

vector<ld> evs;

bool dp[110][(1<<6)+10];

bool correct(ld t,int mask){
    for (int i = 0; i < n; i++)
        for (int j = 0; j < (int)g[i].size(); j++)
            if (g[i][j].l <= t && t <= g[i][j].r)
                if (!!(mask & (1<<i)) == !!(mask & (1<<g[i][j].to)))
                    return false;
    return true;                
}

bool can(ld t,int mask,int mask1){
    for (int i = 0; i < n; i++)
        for (int j = 0; j < (int)g[i].size(); j++)
            if (g[i][j].l <= t && t <= g[i][j].r)
                if (!!(mask & (1<<i)) != !!(mask1 & (1<<i)))
                    return false;
    return true;                
}

bool check(ld upto){
    evs.clear();
    for (int i = 0; i < n; i++)
        g[i].clear();

    for (int i = 0; i < n; i++)
        for (int j = i+1; j < n; j++){
            if (v[i] == v[j]){
                if (abs(p[i] - p[j]) < 5){
                    g[i].pb(rest(0,upto,j));
                    g[j].pb(rest(0,upto,i));
           //         cerr <<"!!!!!"<<endl;
                }
                continue;
            }
            int _i = i, _j = j;
            if (p[_i] > p[_j])
                swap(_i,_j);
            if (v[_i] < v[_j])
                continue;

            ld l = (p[_j] - p[_i] - 5.0)/(v[_i] - v[_j]);
            ld r = (p[_j] - p[_i] - 0.0)/(v[_i] - v[_j]);
            l = max(l,(ld)0.0);
            r = max(r,(ld)0.0);
            if (l > upto)
                continue;
            r = min(r,upto);

            evs.pb(l);
            evs.pb(r);
            evs.pb((l+r)/2);

         //   cerr <<"!!"<<l <<" "<<r<<endl;

            g[i].pb(rest(l,r,j));
            g[j].pb(rest(l,r,i));            
        }

  evs.pb(0);
  evs.pb(upto);
  sort(evs.begin(),evs.end());

  assert(evs.size() <= 110);

  memset(dp,0,sizeof(dp));

  int msk = 0;
  for (int i = 0; i < n; i++)
    msk |= (c[i] << i);

  dp[0][msk] = true;

  for (int i = 0; i < (int)evs.size(); i++)
    for (int j = 0; j < (1<<n); j++){
        if (!dp[i][j] || !correct(evs[i],j))
            continue;
        for (int k = 0; k < (1<<n); k++)
            if (can(evs[i],j,k))
               dp[i+1][k] = true; 
    }

  for (int i = 0; i < (1<<n); i++)
    if (dp[(int)evs.size()][i])
        return true;

  return false;
}

void solve(){
    scanf("%d",&n);
    for (int i = 0; i < n; i++){
        char c;
        scanf(" %c %d %d",&c,&v[i],&p[i]);
        if (c == 'L')
            ::c[i] = 0;
        else
            ::c[i] = 1;
    }

    //check(10.0);
    //return;

    ld l = 0;
    ld r = 100000;

    for (int i = 0; i < 300; i++){
        ld mid = (l+r)/2;
        if (check(mid))
            l = mid;
        else
            r = mid;
    }

    if (l > 20000)
        printf("Possible\n");
    else
        printf("%.10lf\n",(double)((l+r)/2));
}

int main(){
    freopen(INFILE.data(),"r",stdin);
    freopen(OUTFILE.data(),"w",stdout);
    PreCalc();
    int t;
    cin>>t;
    for (int i=1;i<=t;i++)
        {
            printf("Case #%d: ",i);
            solve();
            if (i>t-100 || i%10==0)
                cerr<<"SOLVED:"<<i<<endl;
        }

  return 0;
}