#include <cstdio>
#include <iostream>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <utility>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))
#define SFOR(i,m,n) for((i) = (m); (i) < (n); ++(i))

struct Pos
{
	int p;
	int x, y;
	set<pair<int, int> > A;

public:
	bool operator < (const Pos& a) const
	{
		if (p > a.p) return true;
		if (p == a.p)
		{
			if (x < a.x) return true;
			if (x == a.x)
			{
				if (y < a.y) return true;
				if (y == a.y)
				{
					set<pair<int, int> >::const_iterator it = A.begin();
					set<pair<int, int> >::const_iterator ita = a.A.begin();
					while (it != A.end())
					{
						if (it->first < ita->first || (it->first == ita->first && it->second < ita->second)) return true;
						if (! (it->first == ita->first && it->second == ita->second)) return false;
						it++;
						ita++;
					}
				}
			}
		}
		return false;
	}
};

set<Pos> V;
vector<Pos> Q;

char A[10][6];
char C[10][6];
int n,m,f;

void makecurrent(Pos& s)
{
	memcpy(C, A, sizeof(A));
	set<pair<int, int> >::const_iterator it = s.A.begin();
	while (it != s.A.end())
	{
		C[it->first][it->second] = 1;
		it++;
	}
}

int fall(int x, int y)
{
	int rx = x;
	while (rx < n-1 && C[rx+1][y])
	{
		++rx;
	}
	if (rx - x <= f) return rx;
	return -1;
}

void add(int p, int x, int y, set<pair<int, int> > &A)
{
	Pos np;
	np.p = p;
	np.x = x;
	np.y = y;
	np.A = set<pair<int, int> > (A);
	if (V.find(np) == V.end())
	{
		V.insert(np);
		Q.push_back(np);
		push_heap(Q.begin(), Q.end());
	}
}

int wmain(void)
{
	FILE *fi, *fo;	
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");
	
	int t, tt;
	fscanf(fi, "%d", &tt);
	FOR(t, tt)
	{
		int i,j;
		fscanf(fi, "%d%d%d", &n, &m, &f);
		FOR(i,n) FOR(j,m)
		{
			char ch;
			do{fscanf(fi, "%c", &ch);} while (ch != '.' && ch != '#');
			A[i][j] = (ch == '.');
		}
		V.clear();
		Q.clear();

		Pos s;
		s.p = 0;
		s.x = 0;
		s.y = 0;
		s.A = set<pair<int, int> >();
		V.insert(s);
		Q.push_back(s);
		int res = 1000000000;

		while (!Q.empty())
		{
			s = Q[0];
			if (s.x == n-1) {res = s.p; break;}
			pop_heap(Q.begin(), Q.end());
			Q.pop_back();

			makecurrent(s);

			int x,y;

			// Go left
			x = s.x;
			y = s.y;
			if (y > 0 && C[x][y-1])
			{
				--y;
				x = fall(x, y);
				if (x != -1) add(s.p, x, y, s.A);
			}

			// Go right
			x = s.x;
			y = s.y;
			if (y < m - 1 && C[x][y+1])
			{
				++y;
				x = fall(x, y);
				if (x != -1) add(s.p, x, y, s.A);
			}

			// Dig left
			x = s.x;
			y = s.y;
			if (y > 0 && C[x][y-1] && !C[x+1][y-1])
			{
				set<pair<int, int> > ns(s.A);
				ns.insert(make_pair(x+1, y-1));
				add(s.p + 1, x, y, ns);
			}

			// Dig right
			x = s.x;
			y = s.y;
			if (y < m - 1 && C[x][y+1] && !C[x+1][y+1])
			{
				set<pair<int, int> > ns(s.A);
				ns.insert(make_pair(x+1, y+1));
				add(s.p + 1, x, y, ns);
			}
		}
		if (res < 1000000000)
		{
			fprintf(fo, "Case #%d: Yes %d\n", t+1, res);
		}
		else
		{
			fprintf(fo, "Case #%d: No\n", t+1);
		}
	}

	fclose(fi);
	fclose(fo);

	return 0;
}