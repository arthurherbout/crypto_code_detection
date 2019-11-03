//#pragma comment(linker, "/STACK:134217728")

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
#include <numeric>
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

#define MAX (1 << 9)
#define CNT 74
#define INF 1e47

int P[MAX];
int S[MAX];
int L[MAX];
int LS[MAX];
int LI[MAX];
int R[MAX];
int RS[MAX];
int RI[MAX];
double V[MAX][MAX][2];

double check(int cntR, int cntL, int Y)
{
	int i, j, k, l;
	FOR(i, 0, cntR + 1)
		FOR(j, 0, cntL + 1)
		FOR(k, 0, 2)
		V[i][j][k] = INF;

	V[0][0][0] = 0;
	FOR(i, 0, cntR + 1)
		FOR(j, 0, cntL + 1)
		FOR(k, 0, 2)
		if (V[i][j][k] != INF)
		{
			double time = V[i][j][k];
			double pos = 0;
			if (k == 0 && i != 0)
			{
				FOR(l, 0, i)
				{
					int a = RI[l];
					pos = max(pos, R[a] + RS[a] * time);
				}
			}
			if (k == 1 && j != 0)
			{
				FOR(l, 0, j)
				{
					int a = LI[l];
					pos = L[a] + LS[a] * time;
				}
			}

			if (i != cntR)
			{
				int a = RI[i];
				double dist = R[a] + RS[a] * time;
				if (k == 0)
					dist -= pos;
				else
					dist += pos;

				if (dist < 0)
					dist = 0;

				double v = time + dist / (Y - RS[a]);
				if (v < V[i + 1][j][0])
					V[i + 1][j][0] = v;
			}

			if (j != cntL)
			{
				int a = LI[j];
				double dist = L[a] + LS[a] * time;
				if (k == 1)
					dist -= pos;
				else
					dist += pos;

				if (dist < 0)
					dist = 0;

				double v = time + dist / (Y - LS[a]);
				if (v < V[i][j + 1][1])
					V[i][j + 1][1] = v;
			}
		}

	return min(V[cntR][cntL][0], V[cntR][cntL][1]);
}

int SolveTest(int test)
{
	int Y, N;
	scanf("%d%d", &Y, &N);

	int i, j, k;
	FOR(i, 0, N)
		scanf("%d", &P[i]);
	FOR(i, 0, N)
		scanf("%d", &S[i]);

	int cntR = 0;
	int cntL = 0;
	FOR(i, 0, N)
	{
		if (P[i] < 0)
		{
			L[cntL] = -P[i];
			LS[cntL] = S[i];
			++cntL;
		}
		else
		{
			R[cntR] = P[i];
			RS[cntR] = S[i];
			++cntR;
		}
	}

	double Min = 0, Max = 1e11;
	FOR(i, 0, CNT)
	{
		double Mid = (Max + Min) / 2;

		vector < pair<double, int> > v;
		FOR(j, 0, cntR)
			v.push_back(MP(R[j] + RS[j] * Mid, j));
		
		sort(ALL(v));
		reverse(ALL(v));
		FOR(j, 0, cntR)
			RI[j] = v[j].second;

		vector < pair<double, int> > w;
		FOR(j, 0, cntL)
			w.push_back(MP(L[j] + LS[j] * Mid, j));

		sort(ALL(w));
		reverse(ALL(w));
		FOR(j, 0, cntL)
			LI[j] = w[j].second;

		if (check(cntR, cntL, Y) > Mid)
			Min = Mid;
		else
			Max = Mid;
	}

	printf("Case #%d: %.11lf\n", test + 1, Max);

	return 0;
}

int main()
{
	freopen("c.in", "r", stdin);
	freopen("c.out", "w", stdout);

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
