
#define _CRT_SECURE_NO_DEPRECATE 

#include <string> 
#include <vector> 
#include <map> 
#include <list> 
#include <set> 
#include <queue> 
#include <iostream> 
#include <sstream> 
#include <stack> 
#include <deque> 
#include <cmath> 
#include <memory.h> 
#include <cstdlib> 
#include <cstdio> 
#include <cctype> 
#include <algorithm> 
#include <utility> 
#include <bitset>

using namespace std; 

#define FOR(i, a, b) for(int i = a; i < b; ++i)
#define RFOR(i, b, a) for(int i = b - 1; i >= a; --i)
#define REP(i, N) FOR(i, 0, N)
#define RREP(i, N) RFOR(i, N, 0)

#define MIN(A, B) ((A) < (B) ? (A) : (B))
#define MAX(A, B) ((A) > (B) ? (A) : (B))
#define ABS(A) ((A) < 0 ? (-(A)) : (A))
#define ALL(V) V.begin(), V.end()
#define SIZE(V) (int)V.size()
#define pb push_back
#define mp make_pair
#define EPS 1e-7
#define Pi 3.14159265358979

typedef long long Long;
typedef unsigned long long ULong;
typedef unsigned int Uint;
typedef unsigned char Uchar;
typedef vector <int> VI;
typedef pair <int, int> PI;

int start[1000];
int end[1000];
int m;
int room[1000];
int cntRoom;
int n;

void getRoom(int was)
{
	REP(i,m)
	{
		if (!(was&(1<<start[i])))
			continue;
		if (!(was&(1<<end[i])))
			continue;

		int nwas1=0;
		int nwas2=0;
		for (int j=start[i];j!=end[i];j=(j+1)%n)
		{
			if (was&(1<<j))
				nwas1|=1<<j;
		}

		nwas1|=1<<end[i];

		nwas2=(was^nwas1) | (1<<start[i]) | (1<<end[i]);

		if (nwas1==was || nwas2==was)
			continue;

		getRoom(nwas1);
		getRoom(nwas2);

		return;
	}

	room[cntRoom++]=was;
}

int col[10];
int res[10];
int resCol;
bool used[10];

bool check(int cntCol)
{
	REP(i,cntRoom)
	{
		memset(used,0,sizeof(used));

		bool valid=true;
		REP(j,n)
			if (room[i]&(1<<j))
			{
				if (col[j]==-1)
				{
					valid=false;
					break;
				}
				used[col[j]]=true;
			}

		if (!valid)
			continue;

		REP(j,cntCol)
			if (!used[j])
				return false;
	}
	return true;
}

void solve(int cur,int cntCol)
{
	if (n-cur + cntCol <= resCol)
		return;

	if (!check(cntCol))
		return;

	if (cur==n)
	{
		if (cntCol>resCol)
		{
			resCol=cntCol;
			memcpy(res,col,sizeof(col));
		}
		return;
	}
	
	RREP(curCol,cntCol+1)
	{
		col[cur]=curCol;
		solve(cur+1,(curCol==cntCol)?cntCol+1:cntCol);
		col[cur]=-1;
	}
}

int main()
{
	freopen("in.txt","r",stdin);
	freopen("out.txt","w",stdout);

	int T;
	cin>>T;
	REP(tests,T)
	{
		scanf("%d%d",&n,&m);
		REP(i,m)
		{
			scanf("%d",&start[i]);
			--start[i];
		}
		REP(i,m)
		{
			scanf("%d",&end[i]);
			--end[i];
		}

		cntRoom=0;

		getRoom((1<<n)-1);

		memset(col,-1,sizeof(col));
		col[0]=0;
		resCol=0;
		solve(1,1);

		printf("Case #%d: %d\n",tests+1,resCol);
		REP(i,n)
			printf("%d%c",res[i]+1,i==n-1?'\n':' ');
	}
	return 0;
}
