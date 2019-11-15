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

int ret;

void add(vector<int>& row, int set, int add)
{
	REP(i,row.sz) if(set & (1<<i))
	{
		for(int dx = -1; dx <= 1; ++dx)
		{
			int x = i + dx;
			if(x < 0 || x >= row.sz) continue;
			row[x] += add;
		}
	}
}

bool allzero(const vector<int>& row)
{
	return count(all(row), 0) == row.sz;
}

bool hasNegative(const vector<int>& row)
{
	REP(i,row.sz) if(row[i] < 0) return true;
	return false;
}

void backtrack(int y, vector<vector<int> >& board, int mid)
{
	if(y == board.sz) 
	{
		if(allzero(board.back()))
			ret = max(ret, mid);		
		return;
	}
	int wid = board[0].sz;
	REP(set,(1<<wid))
	{
		if(y > 0) add(board[y-1], set, -1);
		add(board[y], set, -1);
		if(y + 1 < board.sz) add(board[y+1], set, -1);
		if(
			(y == 0 || allzero(board[y-1])) &&
			!hasNegative(board[y]) &&
			(y+1 == board.sz || !hasNegative(board[y+1]))
			)
		{
			int nextmid = (y == board.sz / 2) ? builtin_popcount(set) : mid;
			backtrack(y+1, board, nextmid);
		}
		if(y > 0) add(board[y-1], set, +1);
		add(board[y], set, +1);
		if(y + 1 < board.sz) add(board[y+1], set, +1);
		
	}
}

int main()
{
	//freopen("input.txt", "r", stdin);
	freopen("c:\\gcj\\c-small-attempt0.in", "r", stdin);
	freopen("c:\\gcj\\c-small-0-1.out", "w", stdout);
	int cases;
	cin >> cases;
	REP(cc,cases)
	{
	//	fprintf(stderr, "%d of %d (%.2lf sec) ..\r", cc+1, cases, timer.get());
		printf("Case #%d: ", cc+1);
		int height, width;
		cin >> height >> width;
		vector<vector<int> > board(height, width);
		REP(i,height) REP(j,width) cin >> board[i][j];
		ret = 0;
		backtrack(0, board, -1);
		printf("%d\n", ret);
	}
	fprintf(stderr, "Total elapsed time %.2lf sec\n", timer.get());
}
