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

int main()
{
	//freopen("input.txt", "r", stdin);
	freopen("c:\\gcj\\A-small-attempt0.in", "r", stdin);
	freopen("c:\\gcj\\a-small-0-1.out", "w", stdout);
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
		fprintf(stderr, "%d of %d (%.2lf sec) ..\r", cc+1, cases, timer.get());
		printf("Case #%d: ", cc+1);
		int n;
		cin >> n;
		vector<int> a(n), b(n), c(n);
		REP(i,n)
			cin >> a[i] >> b[i] >> c[i];
		int ret = 0;
		FOR(satisfy,1,(1<<n))
			if(builtin_popcount(satisfy) > ret)
			{
				int maxA = 0, maxB = 0, maxC = 0;
				REP(i,n) if(satisfy&(1<<i))
				{
					maxA = max(a[i], maxA);
					maxB = max(b[i], maxB);
					maxC = max(c[i], maxC);
				}
				if(maxA + maxB + maxC <= 10000)
					ret = builtin_popcount(satisfy);
			}
		printf("%d\n", ret);

	}
	fprintf(stderr, "Total elapsed time %.2lf sec\n", timer.get());
}
