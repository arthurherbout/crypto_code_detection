#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <string>
using namespace std;

#define FR(i,a,n) for(int (i) = (a); (i)<(n); (i)++)
#define RF(i,a,n) for(int (i) = int(n)-1; (i)>=(a); (i)--)
#define FOR(i,n) FR(i,0,n)
#define ROF(i,n) RF(i,0,n)

typedef long long ll;
typedef long double ld;
typedef pair<int,int> pii;
typedef pair<ll,ll> pll;
typedef vector<int> vi;

int dr[4] = {-1,0,1,0};
int dc[4] = {0,1,0,-1};
struct State
{
	int r,c;
	int pr, pc;
	int dist;
	bool operator<(const State& o) const
	{
		return dist>o.dist;
	}
};
bool V[15][15][16][15];
priority_queue<State> Q;
int left, right;
int R, C;
char G[15][16];

bool in(int r, int c)
{
	if(r<0 || r>=R || c<0 || c>=C)
		return false;
	if(G[r][c]=='#')
		return false;
	return true;
}

void consider(int r, int c, int pr, int pc, int dist)
{
	if(!V[r][c][pr][pc])
	{
		V[r][c][pr][pc] = true;
		State next;
		next.r = r;
		next.c = c;
		next.pr = pr;
		next.pc = pc;
		next.dist = dist;
		Q.push(next);
	}
}
int main()
{
	int TESTS;
	scanf("%d", &TESTS);
	FOR(tests,TESTS)
	{
		memset(V, 0, sizeof(V));
		while(Q.size()) Q.pop();
		scanf("%d%d", &R, &C);
		FOR(r,R)
		{
			scanf("%s", G[r]);
			FOR(c,C)
			{
				if(G[r][c]=='O')
				{
					consider(r,c,15,0,0);
				}
			}
		}
		bool done = false;
		while(Q.size())
		{
			State cur = Q.top();
			Q.pop();
			if(G[cur.r][cur.c]=='X')
			{
				done = true;
				printf("Case #%d: %d\n", tests+1, cur.dist);
				break;
			}
			if(cur.pr==15)
			{
				FOR(d,4)
				{
					int r = cur.r;
					int c = cur.c;
					while(in(r+dr[d], c+dc[d]))
					{
						r+=dr[d];
						c+=dc[d];
					}
					consider(cur.r, cur.c, r, c, cur.dist);
				}
			}
			FOR(d,4)
			{
				int r = cur.r+dr[d];
				int c = cur.c+dc[d];
				if(in(r,c))
					consider(r,c,cur.pr, cur.pc, cur.dist+1);
				else if(cur.pr!=15)
					consider(cur.pr, cur.pc, 15, 0, cur.dist+1);
			}
		}
		if(!done)
			printf("Case #%d: THE CAKE IS A LIE\n", tests+1);
	}
}
