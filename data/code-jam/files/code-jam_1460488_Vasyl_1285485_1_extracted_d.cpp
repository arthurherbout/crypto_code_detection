#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cctype>
#include <cstring>
#include <vector>
#include <list>
#include <queue>
#include <deque>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

typedef long long Int;
typedef pair<int,int> PII;
typedef vector<int> VInt;

#define FOR(i, a, b) for(i = (a); i < (b); ++i)
#define RFOR(i, a, b) for(i = (a) - 1; i >= (b); --i)
#define CLEAR(a, b) memset(a, b, sizeof(a))
#define SIZE(a) int((a).size())
#define ALL(a) (a).begin(),(a).end()
#define PB push_back
#define MP make_pair

#define INF (1 << 30)

char A[64][64];

int gcd(int a, int b){ return a == 0 ? b : gcd(b % a, a); }

int SolveTest(int test)
{
	int n, m, d;
	scanf("%d%d%d", &n, &m, &d);

	int i, j;
	FOR(i, 0, n)
		scanf("%s", A[i]);

	int startX = -1;
	int startY = -1;
	FOR(i, 0, n)
		FOR(j, 0, m)
			if(A[i][j] == 'X')
			{
				startX = i;
				startY = j;
			}

	int res = 0;
	FOR(i, -d, d + 1)
		FOR(j, -d, d + 1)
			if(gcd(abs(i), abs(j)) == 1)
			{
				int x = startX;
				int y = startY;
				int dx = i == 0 ? 0 : (i < 0 ? -1 : 1);
				int dy = j == 0 ? 0 : (j < 0 ? -1 : 1);
				int adx = abs(i);
				int ady = abs(j); 
				int r = 2*max(1, adx)*max(1, ady);
				int rx = r/2;
				int ry = r/2;

				Int limit = d*d;
				limit *= r*r;
				limit /= adx*adx + ady*ady;

				Int time = 0;
				while(time*time <= limit)
				{
					int t = INF;
					if(time != 0 && x == startX && y == startY && rx <= r/2 && ry <= r/2)
					{ 
						if(adx != 0)
							t = min(t, (r/2 - rx)/adx);
						if(ady != 0)
							t = min(t, (r/2 - ry)/ady);

						rx += t*adx;
						ry += t*ady;
						time += t;
						if(rx == r/2 && ry == r/2)
						{
							if(time*time <= limit)
							{
//								printf("%d %d\n", i, j);
								++res;
							}
							
							break;
						}
					}

					t = INF;
					if(adx != 0)
						t = min(t, (r - rx)/adx);
					if(ady != 0)
						t = min(t, (r - ry)/ady);

					rx += t*adx;
					ry += t*ady;
					time += t;
					
					int xx = x;
					int yy = y;
					if(rx == r)
						xx += dx;
					if(ry == r)
						yy += dy;

					if(A[xx][yy] != '#')
					{
						if(rx == r)
							rx = 0;
						if(ry == r)
							ry = 0;

						x = xx;
						y = yy;
						continue;
					}

					if(x == xx)
					{
						ry = 0;
						dy = -dy;
						continue;
					}

					if(y == yy)
					{
						rx = 0;
						dx = -dx;
						continue;
					}

					int nx = A[xx][y] == '#' ? 1 : 0;
					int ny = A[x][yy] == '#' ? 1 : 0;
					if(nx == 0 && ny == 0)
						break;

					rx = ry = 0;
					if(nx != 0)
						dx = -dx;
					else
						x = xx;

					if(ny != 0)
						dy = -dy;
					else
						y = yy;
				}
			}

	printf("Case #%d: %d\n", test + 1, res);
	return 0;
}

int main()
{
	freopen("d.in", "r", stdin);
	freopen("d.out", "w", stdout);

	int T, t;
	char buf[1 << 7];
	gets(buf);
	sscanf(buf, "%d", &T);
	FOR(t, 0, T)
	{
		fprintf(stderr, "Solving %d/%d\n", t + 1, T);
		SolveTest(t);
	}

	return 0;
};
