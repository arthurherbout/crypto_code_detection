#include <algorithm>
#include <cstdio>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <map>
#include <queue>

using namespace std;

#define FOR(i,n) for(i = 0; i < (n); ++i)
#define SFOR(i,m,n) for(i = (m); i < (n); ++i)

#define lint __int64

char A[15][15];

int D[15][15][15][15];

char TA[15][15];

int dx[4] = {0,1,0,-1};
int dy[4] = {1,0,-1,0};

int QX[225], QY[225];
int qb, qe;

int R[15][15][15][15][5];

struct Pos
{
	int val,x,y,xp,yp,dir;
};

vector<Pos> Q;

bool cmp(Pos& a, Pos& b)
{
	return a.val > b.val;
}

int n,m;

void doit(Pos& nextp)
{
				if (nextp.x >= 0 && nextp.y >= 0 && nextp.x < n && nextp.y < m && A[nextp.x][nextp.y] == 0)
				{
					if (nextp.val < R[nextp.x][nextp.y][nextp.xp][nextp.yp][nextp.dir] || R[nextp.x][nextp.y][nextp.xp][nextp.yp][nextp.dir] == -1)
					{
						R[nextp.x][nextp.y][nextp.xp][nextp.yp][nextp.dir] = nextp.val;
						Q.push_back(nextp);
						push_heap(Q.begin(), Q.end(), cmp);
					}
				}
}

int main()
{
	FILE *fi, *fo;
	fi = fopen("p1.in", "rt");
	fo = fopen("p1.out", "wt");

	int t,tt;
	fscanf(fi, "%d", &tt);
	for(t = 0; t < tt; ++t)
	{
		int i,j,k;
		fscanf(fi, "%d%d",&n,&m);
		int xs,ys,xt,yt;

		FOR(i,n) FOR(j,m){
			char ch;
			do { fscanf(fi,"%c",&ch);} while (! (ch == '.' || ch =='#' || ch == 'O' || ch == 'X'));
			if (ch == 'O')
			{
				xs = i;
				ys = j;
				ch = '.';
			}
			if (ch == 'X')
			{
				xt = i;
				yt = j;
				ch = '.';
			}
			A[i][j] = ((ch == '#') ? -1 : 0);
		}

		memset(D, 0xff, sizeof(D));

		FOR(i,n) FOR(j,m) if (A[i][j] == 0)
		{
			memcpy(TA, A, sizeof(TA));
			qb = 0;
			qe = 1;
			QX[0] = i;
			QY[0] = j;
			D[i][j][i][j] = 0;
			TA[i][j] = -1;
			while (qb < qe)
			{
				FOR(k,4) if (QX[qb] + dx[k] >= 0 && QX[qb] + dx[k] < n && QY[qb] + dy[k] >= 0 && QY[qb] + dy[k] < m && TA[QX[qb] + dx[k]][QY[qb] + dy[k]] == 0)
				{
					TA[QX[qb] + dx[k]][QY[qb] + dy[k]] = -1;
					D[i][j][QX[qb] + dx[k]][QY[qb] + dy[k]] = D[i][j][QX[qb]][QY[qb]] + 1;
					QX[qe] = QX[qb] + dx[k];
					QY[qe] = QY[qb] + dy[k];
					++qe;
				}
				++qb;
			}
		}

		memset(R, 0xff, sizeof(R));

		Q.clear();

		Pos curp, nextp;
		curp.x = xs;
		curp.y = ys;
		curp.xp = 0;
		curp.yp = 0;
		curp.dir = 4;
		curp.val = 0;
		Q.push_back(curp);
		R[xs][ys][0][0][4] = 0;

		while (!Q.empty())
		{
			curp = Q[0];
			if (curp.x == xt && curp.y == yt) break;
			pop_heap(Q.begin(), Q.end(), cmp);
			Q.pop_back();
			FOR(i,4)
			{
				nextp.x = curp.x + dx[i];
				nextp.y = curp.y + dy[i];
				nextp.xp = curp.xp;
				nextp.yp = curp.yp;
				nextp.dir = curp.dir;
				nextp.val = curp.val + 1;
				doit(nextp);

				int x = curp.x, y = curp.y;
				while (x >= 0 && y >= 0 && x < n && y < m && A[x][y] == 0)
				{
					x += dx[i];
					y += dy[i];
				}
				x -= dx[i];
				y -= dy[i];

				if (curp.dir == 4)
				{
					nextp.x = curp.x;
					nextp.y = curp.y;
					nextp.xp = x;
					nextp.yp = y;
					nextp.dir = i;
					nextp.val = curp.val;
					doit(nextp);
				}
				else
				{
					if (x == curp.xp && y == curp.yp) continue;
					int d;
					
					d = min(D[curp.x][curp.y][x][y], D[curp.x][curp.y][curp.xp][curp.yp] + 1);
					nextp.x = x;
					nextp.y = y;
					nextp.xp = x;
					nextp.yp = y;
					nextp.dir = i;
					nextp.val = curp.val + d;
					doit(nextp);
					
					nextp.x = x;
					nextp.y = y;
					nextp.xp = curp.xp;
					nextp.yp = curp.yp;
					nextp.dir = curp.dir;
					nextp.val = curp.val + d;
					doit(nextp);				


					d = min(D[curp.x][curp.y][x][y] + 1, D[curp.x][curp.y][curp.xp][curp.yp]);					
					nextp.x = curp.x;
					nextp.y = curp.y;
					nextp.xp = x;
					nextp.yp = y;
					nextp.dir = i;
					nextp.val = curp.val + d;
					doit(nextp);
					
					nextp.x = curp.x;
					nextp.y = curp.y;
					nextp.xp = curp.xp;
					nextp.yp = curp.yp;
					nextp.dir = curp.dir;
					nextp.val = curp.val + d;
					doit(nextp);
				}
			
			}
		}

		if (Q.empty())
		{
			fprintf(fo, "Case #%d: THE CAKE IS A LIE\n", t+1);
		
		}
		else
		{
			fprintf(fo, "Case #%d: %d\n", t+1, curp.val);
		}
	}


	fclose(fi);
	fclose(fo);
	return 0;
}