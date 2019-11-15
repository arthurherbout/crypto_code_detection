#include <algorithm>
#include <bitset>
#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <complex>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>

using namespace std;
const long double PI = acos((-1.0l));
template<class T> void chmax(T & a, const T & b) { a = max(a, b); }
template<class T> void chmin(T & a, const T & b) { a = min(a, b); }

#define REP(i,n) for(int i=0; i<(n); i++)
#define FOR(i,a,b) for(int i=(a); i<(b); i++)
#define FORD(i,a,b) for(int i=(b)-1; i>=(a); i--)
#define FORV(i,v) for(int i=0; i<(v).size(); i++)
#define FORVD(i,v) for(int i=(v).size()-1; i>=0; i--)
#define FORE(it,c) for (__typeof((c).begin()) it=(c).begin(); it!=(c).end(); it++)
#define FORED(it,c) for (__typeof((c).rbegin()) it=(c).rbegin(); it!=(c).rend(); it++)
#define DEBUG(x) cout << '>' << #x << ':' << (x) << endl
#define SIZE(a) ((int)(a).size())
#define CLEAR(a) memset((a),0,sizeof(a))
#define ALL(a) (a).begin(),(a).end()




int t;
int n;
vector <pair <int, int> > V;
vector <int> Reach;

int main()
{
    //freopen("a.in", "r", stdin);
    cin >> t;
    FOR(e,1,t+1)
    {
        cin >> n;
        //DEBUG(n);
        V.clear();
        int d, d0, le, le0, dist;
        cin >> d0 >> le0;
        V.push_back(make_pair(d0, le0));
        REP(i,n-1)
        {
            cin >> d >> le;
            V.push_back(make_pair(d, le));
        }
        cin >> dist;
        V.push_back(make_pair(dist, 0));
        n++;
        sort(ALL(V));
        Reach.clear();
        Reach.resize(n, -1);
        REP(i,n) if(V[i].first == d0 && V[i].second == le0) Reach[i] = 0;
        int rindex = 0;
        //DEBUG(n);
        REP(i,n)
        {
            if(Reach[i] == -1) continue;
            //DEBUG(i);
            int newreach = min(2*V[i].first - Reach[i], V[i].first+V[i].second);
            int from = V[i].first;
            //DEBUG(newreach);
            while(rindex < n)
            {
                if(V[rindex].first <= newreach)
                {
                    Reach[rindex] = from;
                    rindex++;
                }
                else
                    break;
            }
        }
        cout << "Case #" << e << ": " << (Reach[n-1] == -1 ? "NO" : "YES") << endl;
    }
    return 0;
}
