#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <queue>
#include <cstring>

using namespace std;

#define FOREACH(it,c) for( __typeof((c).begin()) it=(c).begin();it!=(c).end();it++)
#define FOR(i,a,b) for( int i=(a),_b=(b);i<=_b;i++) 
#define DOW(i,b,a) for( int i=(b),_a=(a);i>=_a;i--)
#define REP(i,n) FOR(i,0,(n)-1)
#define DEP(i,n) DOW(i,(n)-1,0)
#define all(a) (a).begin() , (a).end()

typedef vector<int> VI ;
typedef vector<string> VS ;
template<class T> inline int size(const T&c) { return c.size(); }  

long nTest, n, m;
long tot;
string List[10001];
char curr[100];
string Guess;
string result;

long cal(string a, string b)
{
    long x=0, con=a.size();
    bool dd[10001];
    memset(dd, true, sizeof(dd));
    memset(curr, 0, sizeof(curr));
    REP(i, a.size()) curr[i]='-';
    REP(i, n) if (List[i].size()!=a.size()) dd[i]=false;
    REP(i, b.size())
    {
        bool chon=false;
        REP(j, n)
            if (dd[j] && List[j].find(b[i], 0)!=string::npos) {chon=true; break;}
        if (!chon) continue;
        if (a.find(b[i], 0) == string::npos)
        {
            REP(j, n)
                if (List[j].find(b[i], 0) != string::npos) dd[j]=false;
            x++;
        } else
        {
            REP(j, a.size()) if (a[j]==b[i]) {curr[j]=b[i]; con--;}
            REP(j, n) if (dd[j])
            {
                REP(k, a.size()) if (curr[k]!='-' && curr[k]!=List[j][k]) dd[j]=false;                
                REP(k, a.size())
                    if (List[j][k]==b[i] && curr[k]!=b[i]) dd[j]=false;
            }
            if (!con) break;
        }        
    }
    return x;   
}

main()
{
    cin>>nTest;
    REP(kk, nTest)
    {
        cin>>n>>m;
        getline(cin, Guess);
        REP(i, n) getline(cin, List[i]);
        cout<<"Case #"<<kk+1<<":";
        REP(i, m)
        {
            tot=-1;
            result="";
            getline(cin, Guess);
            long a;
            REP(j, n) {a=cal(List[j], Guess); if (a>tot) {result=List[j]; tot=a;}}
//            cout<<result<<" "<<tot<<endl;
            cout<<" "<<result;
        }
        cout<<endl;
    }
    return 0;
}