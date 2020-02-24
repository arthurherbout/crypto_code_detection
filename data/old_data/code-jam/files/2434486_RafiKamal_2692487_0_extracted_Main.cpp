#include <cstdio>
#include <cctype>
#include <algorithm>
#include <cstring>
#include <iostream>
#include <cmath>
#include <stack>
#include <string>     
#include <map>
#include <set>
#include <list>
#include <queue>
#include <utility>
#include <sstream>
#include <iomanip>

#define SIZE 5000
#define MOD 20071027
#define clr(a) memset(a, 0, sizeof a)
#define reset(a) memset(a, -1, sizeof a)

#define BOUNDARY(i, j) ((i >= 0 && i < row) && (j >= 0 && j < column))

#define max3(x, y, z) max(max(x, y), max(y, z)) 

using namespace std;

int n, m, row, column;

int dp[101][20005];
int b[105];
int mx;

int solve(int pos, int mote) {
	if(pos == n || mote > mx) return 0;
	if(dp[pos][mote] == -1) {
		int next = b[pos+1];
		int first = MOD;
		if(mote > next)
			first = solve(pos+1, mote+next);

		int insert = MOD;
		if(2*mote-1 != mote) insert = solve(pos, 2*mote - 1);
		else if(2*mote-2 > mote) insert = solve(pos, 2*mote - 2);
		int del = solve(pos+1, mote);
		dp[pos][mote] = min(first, 1 + min(insert, del));

	}
	
	return dp[pos][mote];
}
	

int main()
{	
	freopen("input.txt", "r", stdin);
	freopen("out.txt", "w", stdout);
	
	int t, tc, i, j, a, k;
	cin >> tc;
	for(t = 1; t <= tc; t++) {
		cin >> a >> n;
		for(i = 1; i <= n; i++) cin >> b[i];
		sort(b+1, b+n+1);
		mx = b[n];
		reset(dp);
		int res = solve(0, a);
		printf("Case #%d: %d\n", t, res);
	}
			   
	return 0;
}