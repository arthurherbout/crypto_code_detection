#include <set>
#include <vector>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define FOR(i,n) for((i) = 0; (i) < (n); ++(i))
#define SFOR(i,m,n) for((i) = (m); (i) < (n); ++(i))

FILE *fi, *fo;

int n, m;
char A[60][60];
int nc;
int CX[10], CY[10];
int NR[10];

int dx[4] = {-1, 0, 0, 1};
int dy[4] = {0, 1, -1, 0};

set<pair<int, int>> V;
vector<pair<int, int>> Q;
set<set<pair<int, int>>> GV;
vector<set<pair<int, int>>> GQ;

void findR(int cur, int x, int y)
{
	Q.clear();
	V.clear();
	Q.push_back(make_pair(x, y));
	V.insert(Q[0]);
	int qb = 0, i;
	while (qb < Q.size())
	{
		int cx = Q[qb].first;
		int cy = Q[qb].second;
		FOR(i, 3)
		{
			int nx = cx + dx[i];
			int ny = cy + dy[i];
			if (nx >= 0 && ny >= 0 && nx < n && ny < m && A[nx][ny] && V.find(make_pair(nx, ny)) == V.end())
			{
				Q.push_back(make_pair(nx, ny));
				V.insert(Q[Q.size() - 1]);
			}			
		}
		++qb;
	}
	NR[cur] = Q.size();
	sort(Q.begin(), Q.end());
}

string lucky(int cur)
{
	if (V.size() == 1)
	{
		if (V.begin()->first == CX[cur] && V.begin()->second == CY[cur]) return "Lucky";
	}
	GV.clear();
	GQ.clear();
	GV.insert(V);
	GQ.push_back(V);
	int qb = 0, i, j;
	while (qb < GQ.size())
	{
		SFOR(i, 1, 4)
		{
			set<pair<int, int>>& CUR = GQ[qb];
			set<pair<int, int>> NEW;
			int fl = 1;
			for(set<pair<int, int>>::iterator it = CUR.begin(); it != CUR.end(); ++it)
			{
				int nx = it->first + dx[i];
				int ny = it->second + dy[i];
				if (nx < 0 && ny < 0 && nx >= n && ny >= m || (A[nx][ny] == 0))
				{
					nx = it->first;
					ny = it->second;
				}
				pair<int, int> NP = make_pair(nx, ny);
				if (nx > CX[cur])
				{
					fl = 0; break;
				}
				NEW.insert(NP);
			}
			if (fl)
			{
				if (NEW.size() == 1)
				{
					if (NEW.begin()->first == CX[cur] && NEW.begin()->second == CY[cur]) return "Lucky";
				}
				if (GV.find(NEW) == GV.end())
				{
					GV.insert(NEW);
					GQ.push_back(NEW);
				}
			}
		}
		++qb;
	}
	return "Unlucky";
}

int countR(int i)
{
	return NR[i];
}

void mainImpl(int _c)
{
	int i, j, k;
	fscanf(fi, "%d%d\n", &n, &m);
	nc = 0;
	FOR(i, n) 
	{
		char buf[100];
		fgets(buf, 100, fi);
		FOR(j, m)
		{
			char ch = buf[j];
			if (ch == '#') A[i][j] = 0; else A[i][j] = 1;
			if (ch >= '0' && ch <= '9') 
			{
				nc = max(nc, ch - '0' + 1);
				CX[ch - '0'] = i;
				CY[ch - '0'] = j;
			}
		}
	} 
	fprintf(fo, "Case #%d:\n", _c);
	FOR(i, nc)
	{
		findR(i, CX[i], CY[i]);
		fprintf(fo, "%d: %d %s\n", i, countR(i), lucky(i).c_str());
	}
}


int main()
{
	fi = fopen("a.in", "rt");
	fo = fopen("a.out", "wt");

	int t, tt;
	fscanf(fi, "%d", &tt);

	FOR(t, tt) mainImpl(t + 1);

	fclose(fo);
	fclose(fi);
	return 0;
}