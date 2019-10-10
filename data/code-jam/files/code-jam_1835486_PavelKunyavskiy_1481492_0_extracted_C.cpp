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


const ll LINF = 1000000000LL * 1000000000LL;

int M,F,n; 
pair<ll,ll> p[210];

void PreCalc(){
}


ld dcalc(ll times){
   ld ans = 0;
   for (int i = 1;i < n; i++)
      ans += max(0LL,min(times,p[i].first)-p[i-1].first)*(ld)p[i].second; 
   return ans; 
}

ld dcalc(ll times,ll days){
    return dcalc(days/times)*times + dcalc(days/times+1)*(days%times) + F*(ld)times;
}

ll SMUL(ll x,ll y){
    if (x == 0 || y == 0 || x <= (LINF)/y)
        return x * y;
    return LINF;
}


ll SADD(ll x,ll y){
    if (x <= (LINF) - y)
        return x + y;
    return LINF;
}

ll calc(ll times){
   ll ans = 0;
   for (int i = 1;i < n; i++)
      ans = SADD(ans, SMUL(max(0LL,min(times,p[i].first)-p[i-1].first),p[i].second)); 
   return ans; 
}

ll calc(ll times,ll days){
    return SMUL(calc(days/times),times-(days%times)) + SMUL(calc(days/times+1),days%times) + SMUL(F,times);
}  

bool check(int days){
    ll l = 1;
    ll r = days+2;
    while (r-l > 100){
        ll x = l + (r-l)/3;
        ll y = r - (r-l)/3;
        ld f1 = calc(x,days);
        ld f2 = calc(y,days);
        if (f1 < f2)
            r = y;
        else
            l = x;
    }
    for (int i = l; i < r; i++){
        if (calc(i,days) <= M)
            return true;
    }
    return false;
}

void solve(){
    cin >> M >> F >> n;
    for (int i = 0; i < n; i++)
        cin >> p[i].second>>p[i].first,p[i].first++;
    p[n++] = mp(0,0);
    p[n++] = mp(1LL<<60,1LL<<60);

    sort(p,p+n);
    for (int i = n-2; i >= 0; i--)
        p[i].second = min(p[i].second,p[i+1].second);


    ll l = 0, r = M+1;
    while (r-l > 1){
        int mid = (l+r)/2;
        //cerr << mid << endl;
        if (check(mid)){
            l = mid;
        }
        else {
            r = mid;
        }
    }
    cout << l << endl;
    return;
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
            if (i>t-20 || i%10==0)
                cerr<<"SOLVED:"<<i<<endl;
        }

  return 0;
}