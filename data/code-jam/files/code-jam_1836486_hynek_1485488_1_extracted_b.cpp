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
#include <cstring>

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

#define N 128
int t, h, n, m;
int flo[N*N], cei[N*N];
long long dist[N*N];
int final[N*N];
priority_queue <pair <long long, int> > Q;

void update (int fromx, int fromy, int tox, int toy)
{
    int frompos = fromx * N + fromy;
    int topos = tox * N + toy;
    long long fromtime = dist[frompos];
    long long totime = dist[topos];
    int water = h - min((long long) h, dist[frompos]);
    if(final[topos] == 2) return;
    //cout << "updating [" << tox << "," << toy << "]" << endl;
    //DEBUG(topos);
    //DEBUG(frompos);
    //DEBUG(cei[topos]);
    //DEBUG(cei[frompos]);
    //DEBUG(flo[topos]);
    //DEBUG(flo[frompos]);
    //DEBUG(water);
    if(cei[topos] - flo[frompos] < 50) return;
    if(cei[topos] - flo[topos] < 50) return;
    if(cei[frompos] - flo[topos] < 50) return;
    //cout << " fiedl is acessible...\n";
    long long timeToWait = 0;
    if(water > (cei[topos] - 50))
    {
        timeToWait = water - (cei[topos] - 50);
        water -= timeToWait;
    }
    //DEBUG(timeToWait);
    //DEBUG(water);
    long long timeToTravel = 0;
    if(water < h)
    {
        if(water - flo[frompos] >= 20)
        {
            timeToTravel = 10;
        }
        else
        {
            timeToTravel = 100;
        }
    }
    long long newTotime = fromtime + timeToWait + timeToTravel;
    //DEBUG(timeToTravel);
    if(final[topos] == 0 || (final[topos] == 1 && newTotime < totime))
    {
        //cout << "dist set to " << newTotime << endl;
        //DEBUG(fromtime);
        //DEBUG(timeToWait);
        //DEBUG(timeToTravel);
        dist[topos] = newTotime;
        final[topos] = 1;
        Q.push(make_pair(-dist[topos], topos));
    }
}

int main()
{
    //freopen("b.in", "r", stdin);
    cin >> t;
    FOR(e,1,t+1)
    {
        cin >> h >> n >> m;
        REP(i,n)
        {
            REP(j,m)
            {
                cin >> cei[i*N+j];
            }
        }
        REP(i,n)
        {
            REP(j,m)
            {
                cin >> flo[i*N+j];
            }
        }
        CLEAR(final);
        dist[0] = 0;
        final[0] = 1;
        Q.push(make_pair(-dist[0], 0));
        while(!(Q.empty()))
        {
            int pos = Q.top().second;
            int x = pos / N;
            int y = pos % N;
            //cout << "at time " << dist[pos] << " entering field [" << x << "," << y << "]" << endl;
            Q.pop();
            if(final[pos] == 2) continue;
            final[pos] == 2;
            if(x-1 >= 0) update(x,y,x-1,y);
            if(x+1 < n) update(x,y,x+1,y);
            if(y-1 >= 0) update(x,y,x,y-1);
            if(y+1 < m) update(x,y,x,y+1);
        }
        cout << "Case #" << e << ": " << setprecision(12) << ((long double)(dist[(n-1)*N+(m-1)]))/10 << endl;
    }
    return 0;
}
