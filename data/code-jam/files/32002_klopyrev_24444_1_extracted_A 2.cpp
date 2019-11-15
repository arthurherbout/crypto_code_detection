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

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

bool down[6009][6009];
bool lleft[6009][6009];

int ct[6009][6009];

int main()
{
	int TESTS;
	scanf("%d", &TESTS);
	FOR(tests,TESTS)
	{
		fprintf(stderr, "TEST %d\n", tests+1);
		memset(lleft, 0, sizeof(lleft));
		memset(down, 0, sizeof(down));
		memset(ct, 0, sizeof(ct));
		int L;
		scanf("%d", &L);
		int x = 3003;
		int y = 3003;
		int dir = 0;
		FOR(i,L)
		{
			string S;
			int T;
			cin >> S >> T;
			FOR(j,T)
			{
				FOR(k,S.size())
				{
					if(S[k]=='L')
						dir = (dir+3)%4;
					else if(S[k]=='R')
						dir = (dir+1)%4;
					else
					{
						if(dir==0)
							lleft[x][y] = true;
						else if(dir==1)
							down[x][y] = true;
						x+=dx[dir];
						y+=dy[dir];
						if(dir==2)
							lleft[x][y] = true;
						else if(dir==3)
							down[x][y] = true;
					}
				}
			}
		}
		FOR(y,6009)
		{
		int lleftwalls = 0;
			FOR(x,6009)
			{
				if(lleft[x][y])
					lleftwalls++;
				if(lleftwalls%2==0 && lleftwalls!=0)
				{
					ct[x][y]++;
				}
			}
		}
		int res = 0;
		FOR(y,6009)
		{
		int rrightwalls = 0;
			ROF(x,6009)
			{
				if(rrightwalls%2==0 && rrightwalls!=0 && ct[x][y]==1)
				{
					ct[x][y] = 2;
					res++;
				}
				if(lleft[x][y])
					rrightwalls++;
			}
		}
		FOR(x,6009)
		{
		int downwalls = 0;
			FOR(y,6009)
			{
				if(down[x][y])
					downwalls++;
				if(ct[x][y]==2)
				{
					ct[x][y] = 0;
					continue;
				}
				ct[x][y] = 0;
				if(downwalls%2==0 && downwalls!=0)
				{
					ct[x][y]++;
				}
			}
		}
		FOR(x,6009)
		{
		int upwalls = 0;
			ROF(y,6009)
			{
				if(upwalls%2==0 && upwalls!=0 && ct[x][y]==1)
				{
					ct[x][y] = 2;
					res++;
				}
				if(down[x][y])
					upwalls++;
			}
		}
		printf("Case #%d: %d\n", tests+1, res);
	}
}
