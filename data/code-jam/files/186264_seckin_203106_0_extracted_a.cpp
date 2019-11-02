#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <cctype>
#include <string>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime> 
#define all(c) (c).begin(),(c).end()
#define tr(c,i) for(typeof((c).begin()) i = (c).begin(); i != (c).end(); i++)
#define ll long long 
#define sz(a) int((a).size())
#define pb push_back
#define SORT(x) sort((x).begin(),(x).end()); 
#define VI vector<int>
#define VII vector < vector <int> >
#define MP make_pair
#define SET(x,byte) memset(x,byte,sizeof(x));
#define I(x) ((x) - 'A')
#define REP(i,mx) for(int i=0;(i)<(mx);i++)
#define RFOR(i,mx,mn) for(int i=(mx);(i)>=(mn);i--)
#define FORD(i,mn,mx,k) for(int i=(mn);(i)<=(mx);i+=(k))
#define FOR(i,mn,mx) for(int i=mn;(i)<=(mx);i++)
#define U(x) (1<<(x))
#define INF 1000000000
#define DEBUG 0

using namespace std;

string B(vector<char> v){
    SORT(v);
    int i=0;
    while(v[i]=='0') i++;
    string str;
    str+=v[i];
    REP(j,i) str+='0';
    str+='0';
    FOR(j,i+1,sz(v)-1) str+=v[j];
    return str;
}
string yap(string s){
    vector<char> v;
    REP(i,sz(s)) v.pb(s[i]);
    int buyuk=1;
    REP(i,sz(v)-1) if(v[i]<v[i+1]) buyuk=0;
    if(buyuk) return B(v);

    int mn= v[sz(v)-1],tut;
    RFOR(i,sz(v)-2,0) 
        if(v[i]<v[i+1]){
            tut=i;
            break;
        }

    int sag = -1;
    FOR(j,tut+1,sz(v)-1) if(sag==-1 || (v[sag]>v[j] && v[j]>v[tut]  )) sag=j; 
    char tmp = v[sag];
    v[sag] = v[tut];
    v[tut] = tmp;
    
    string str;
    REP(i,tut+1) str+=v[i];
    
    vector<char> v2;
    FOR(i,tut+1,sz(v)-1) v2.pb(v[i]);
    SORT(v2);
    REP(i,sz(v2))str+=v2[i];
    return str;
}
int main(){
    int n;
    string s;
    scanf("%d",&n);
    REP(i,n){
        cin>>s;
        string res;
        if(sz(s)>1)
            res = yap(s);
        else
            res = (s) + "0";
        cout<<"Case #"<<i+1<<": "<<res<<endl;
    }
    return 0;
}
