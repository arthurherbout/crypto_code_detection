#include<iostream>
#include<algorithm>
#include<sstream>
#include<string>
#include<vector>
#include<cmath>
#include<cstdio>
#include<cstdlib>
#include<fstream>
#include<cassert>
#include<numeric>
#include<set>
#include<map>
#include<queue>
#include<list>
#include<deque>
#include<windows.h>
using namespace std;

#define FOR(i,a,b) for(int i = (a); i < (b); ++i)
#define REP(i,n) FOR(i,0,n)
#define FORE(it,x) for(typeof(x.begin()) it=x.begin();it!=x.end();++it)
#define pb push_back
#define all(x) (x).begin(),(x).end()
#define CLEAR(x,with) memset(x,with,sizeof(x))
#define sz size()
typedef long long ll;

template<typename T>
int builtin_popcount(T x) { int ret = 0; while(x) { ++ret; x = ((x-1)&x); } return ret; }
struct Timer
{
	long long start, freq;
	Timer() { QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start); }
	double get() const { long long now; QueryPerformanceCounter((LARGE_INTEGER*)&now); return double(now - start) / double(freq); }
};
static Timer timer;

bool trap[101][101];

int main()
{
	//freopen("input.txt", "r", stdin);
	freopen("c:\\gcj\\B-small-attempt0.in", "r", stdin);
	freopen("c:\\gcj\\b-small-0-1.out", "w", stdout);
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		//fprintf(stderr, "%d of %d (%.2lf sec) ..\r", cc+1, cases, timer.get());
		printf("Case #%d: ", cc+1);
		int width, height;
		cin >> width >> height;
		CLEAR(trap,0);
		int dx[2], dy[2];
		REP(i,2) cin >> dx[i] >> dy[i];
		int x, y;
		cin >> x >> y;
		queue<pair<int,int> > q;
		q.push(make_pair(y, x));
		trap[y][x] = true;
		int ret = 0;
		while(!q.empty())
		{
			int y = q.front().first, x = q.front().second;
			q.pop();
			++ret;
			REP(k,2)
			{
				int ny = y + dy[k], nx = x + dx[k];
				if(ny < 0 || nx < 0 || ny >= height || nx >= width) continue;
				if(trap[ny][nx]) continue;
				trap[ny][nx] = true;
				q.push(make_pair(ny, nx));
			}
		}
		printf("%d\n", ret);
	}
	fprintf(stderr, "Total elapsed time %.2lf sec\n", timer.get());
}
