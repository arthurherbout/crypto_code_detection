/*	SURENDRA KUMAR MEENA	*/
#include <vector>
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
#include <queue>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <climits>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;
typedef long long int LL;
#define ALL(s) (s).begin(),(s).end()
#define R(i,m,n)	for(int i=m;i>=n;i--)
#define FF(i,m,n)	for(int i=m;i<n;i++)
#define F(i,n)	FF(i,0,n)
#define VI vector<int>
#define PB push_back
#define CLR(s,v) memset(s,v,sizeof(s))
string to_str(LL x){ ostringstream o;o<<x;return o.str();}
LL to_int(string s){ istringstream st(s); LL i;	st>>i;return i;}
#define FR(it,t) for(typeof(t.begin()) it=t.begin(); it!=t.end(); ++it)
typedef pair<int,int> PI;
#define f first
#define s second

int n;
double mem[(1<<20)+10];
bool memo[(1<<20)+10];
int maxMask;
double getFn(int mask) {
    if(maxMask==mask)   return 0.0;
    if(memo[mask])  return mem[mask];
    memo[mask] = true;
    double& ret = mem[mask];
    ret = 0.0;
    int d=0;
    F(i,n) {
        if((mask & (1<<i))==0) {
            d = i;
            break;
        }
    }
    vector<int> dist = vector<int>(n,0);
    R(i,n-1,0) {
        if((mask & (1<<i))) {
            dist[i] = ++d;
        } else {
            dist[i] = 0;
            d = 0;
        }
    }
    
    F(i,n) {
        ret += ((n-dist[i])+getFn((mask|(1<<((i+dist[i])%n)))));
    }
    ret /= n;
    return ret;
}

int main(){
    int t;
    cin>>t;
    FF(kase,1,t+1){
        cout<<"Case #"<<kase<<": ";
        CLR(memo,0);
        string s;
        cin>>s;
        n = s.size();
        int mask = 0;
        F(i,s.size()) {
            if(s[i]=='X') {
                mask |= (1<<i);
            }
        }
        maxMask = (1<<n)-1;
        printf("%.11f\n",getFn(mask));
    }
    return 0;
}
