#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
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
#include <ctime>

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

const long long MAX = 10000000000ll;
int t, n, sum;
vector <int> judges;
vector <long long> points;

int main()
{
    //freopen("a.in", "r", stdin);
    cin >> t;
    FOR(e,1,t+1)
    {
        cin >> n;
        judges.resize(n);
        points.resize(n);
        sum = 0;
        REP(i,n)
        {
            cin >> judges[i];
            sum += judges[i];
        }
        REP(i,n)
        {
            points[i] = ((long double)(judges[i])) / sum * MAX;
        }
        cout << "Case #" << e << ":";
        REP(i,n)
        {
            long long saveVal = MAX;
            long long notSave = -1ll;
            while(notSave + 1 < saveVal)
            {
                long long mid = (saveVal + notSave) / 2;
                long long iTotal = points[i] + mid;
                long long neededVotes = 0;
                REP(j,n)
                {
                    long long jNeeds = iTotal + 1 - points[j];
                    if(jNeeds > 0)
                        neededVotes += jNeeds;
                }
                if(neededVotes > MAX)
                {
                    saveVal = mid;
                }
                else
                {
                    notSave = mid;
                }
            }
            cout << " " << setprecision(8) << ((long double)(saveVal)) / (MAX/100);
        }
        cout << endl;
    }
    return 0;
}
