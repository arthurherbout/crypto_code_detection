#include <cstdio>
#include <algorithm>
#include <vector>
#include <cassert>
#include <map>
#include <set>
#include <cmath>
#include <stack>
#include <queue>
#include <string>
#include <iostream>
using namespace std;
#define sz(a) (int)((a).size())
#define pb push_back
#define mp make_pair
#define all(a) (a).begin(), (a).end()
typedef pair<int, int> pii;
typedef vector<int> vint;
typedef long long lint;

const int INF = 1 << 20;

int R, C, F;
char A[55][55];
int D[55][55];
inline bool Valid(int r, int c)
{
	return r >= 0 && r < R && c >= 0 && c < C && A[r][c] == '.';
}
inline bool Valid2(int r, int c)
{
	return r >= 0 && r < R && c >= 0 && c < C;
}

bool Solve(int test)
{
	scanf("%d %d %d", &R, &C, &F);
	for(int r = 0; r < R; ++r)
		scanf("%s", A[r]);
	for(int r = 0; r < R; ++r)
		for(int c = 0; c < C; ++c)
			D[r][c] = INF;
	D[0][0] = 0;
	for(int o = 0; o < 2; ++o) for(int ii = 0; ii < R; ++ii) for(int jj = 0; jj < C; ++jj)
	{
		for(int r = 0; r < R; ++r)
			for(int c = 0; c < C; ++c)
			{
				if(D[r][c] == INF)
					continue;
				int rr = r, cc = c - 1;
				if(Valid2(rr, cc))
				{
					int cost = 0;
					if(A[rr][cc] == '#')
					{
						cost = INF;
						if(Valid2(r - 1, c) && D[r - 1][c] < INF)
							cost = 1;
					}
					while(rr + 1 < R && A[rr + 1][cc] == '.')
						rr++;
					if(rr - r <= F)
					{
						D[rr][cc] = min(D[rr][cc], D[r][c] + cost);
					}
				}
				rr = r, cc = c + 1;
				if(Valid2(rr, cc))
				{
					int cost = 0;
					if(A[rr][cc] == '#')
					{
						cost = INF;
						if(Valid2(r - 1, c) && D[r - 1][c] < INF)
							cost = 1;
					}
					while(rr + 1 < R && A[rr + 1][cc] == '.')
						rr++;
					if(rr - r <= F)
					{
						D[rr][cc] = min(D[rr][cc], D[r][c] + cost);
					}
				}

				//dig
				rr = r, cc = c - 1;
				if(Valid2(rr, cc) && rr + 1 < R && A[rr + 1][cc] == '#')
				{
					int cost = 0;
					if(A[rr][cc] == '#')
					{
						cost = INF;
						if(Valid2(r - 1, c) && D[r - 1][c] < INF)
							cost = 1;
					}
					rr++;
					while(rr + 1 < R && A[rr + 1][cc] == '.')
						rr++;
					if(rr - r <= F)
						D[rr][cc] = min(D[rr][cc], D[r][c] + 1 + cost);
				}

				rr = r, cc = c + 1;
				if(Valid2(rr, cc) && rr + 1 < R && A[rr + 1][cc] == '#')
				{
					int cost = 0;
					if(A[rr][cc] == '#')
					{
						cost = INF;
						if(Valid2(r - 1, c) && D[r - 1][c] < INF)
							cost = 1;
					}
					rr++;
					while(rr + 1 < R && A[rr + 1][cc] == '.')
						rr++;
					if(rr - r <= F)
						D[rr][cc] = min(D[rr][cc], D[r][c] + 1 + cost);
				}
				
			}
	}
	int ans = INF;
	for(int c = 0; c < C; ++c)
		ans = min(ans, D[R - 1][c]);
	printf("Case #%d: ", test);
	if(ans < INF)
		printf("Yes %d\n", ans);
	else
		puts("No");
	return true;
}

int main()
{
#ifndef ONLINE_JUDGE
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);
#endif
	int t = 0;
	scanf("%d", &t);
	for(int i = 1; i <= t; ++i)
		Solve(i);
	//*/while(Solve(++t));
	return 0;
}