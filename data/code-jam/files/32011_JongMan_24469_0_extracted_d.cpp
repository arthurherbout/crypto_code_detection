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

struct QueueEntry
{
	int cost;
	int y, x;
	int treeY, treeX;
	QueueEntry(int cost, int y, int x, int treeY, int treeX) : cost(cost), y(y), x(x), treeY(treeY), treeX(treeX)
	{		
	}	
};
bool operator < (const QueueEntry& a, const QueueEntry& b)
{
	return a.cost > b.cost;
}

int get(vector<int>& p, int here)
{
	return p[here] == here ? here : (p[here] = get(p, p[here]));
}

const int dx[4] = { 1, -1, 0, 0};
const int dy[4] = { 0, 0, 1, -1 };

int main()
{
	//freopen("input.txt", "r", stdin);
	freopen("c:\\gcj\\d-small-attempt0.in", "r", stdin);
	freopen("c:\\gcj\\d.out", "w", stdout);
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		//fprintf(stderr, "%d of %d (%.2lf sec) ..\r", cc+1, cases, timer.get());
		printf("Case #%d: ", cc+1);
		int height, width;
		cin >> height >> width;
		vector<string> m(height);		
		REP(i,height) cin >> m[i];
		priority_queue<QueueEntry> pq;
		int dist[31][31];
		CLEAR(dist,-1);
		pq.push(QueueEntry(0, 0, 0, 0, 0));		
			
		int ret = 0;
		while(!pq.empty())
		{
			QueueEntry here = pq.top();
			pq.pop();
			if(dist[here.y][here.x] != -1) continue;
			dist[here.y][here.x] = here.cost;
			//printf("(%d,%d) connected from (%d,%d) with a cost of %d\n", here.y, here.x, here.treeY, here.treeX, here.cost);
			ret += here.cost;
			REP(k,4)
			{
				int ny = here.y + dy[k], nx = here.x + dx[k];
				if(ny < 0 || nx < 0 || ny >= height || nx >= width) continue;
				if(m[ny][nx] == '.') continue;
				if(dist[ny][nx] != -1) continue;
				if(m[here.y][here.x] == 'T')
					pq.push(QueueEntry(1, ny, nx, ny, nx));
				else
					pq.push(QueueEntry(1 + here.cost, ny, nx, here.treeY, here.treeX));
			}
		}
		printf("%d\n", ret);
	}
	fprintf(stderr, "Total elapsed time %.2lf sec\n", timer.get());
}
