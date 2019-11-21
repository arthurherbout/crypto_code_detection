#include <bits/stdc++.h>

#define INF 2147483647
#define LLD int
#define clr(a) memset(a,0,sizeof(a))
#define reset(a) memset(a,-1,sizeof(a))

#define PRINT(X) (cout << #X << " -> " << X << endl)
#define PI acos(0)
#define MAX_INT 2147483647
#define SIZE 1000005
#define MOD 1000000007

#define BD(i, j) (i >= 0 && i < n && j >= 0 && j < m)

using namespace std;

typedef pair<int, int> Event;

int M[SIZE];

int R, C, N;

int countUnhappiness(int mask) {
	int unhappiness = 0;
	for (int i = 0; i < R * C; i++) {
		if (mask & (1 << i)) {
			if (i % C != C - 1) {
				if (mask & (1 << (i + 1)))
					unhappiness++;
			}
			if (i / C != R - 1) {
				if (mask & (1 << (i + C)))
					unhappiness++;
			}
		}
	}
	
	return unhappiness;
}

int solve(int mask, int n, int pos) {
	if (n == 0)
		return countUnhappiness(mask);
	else if (n > R * C - pos)
		return INT_MAX;
	else {
		int taken = solve(mask | (1 << pos), n - 1, pos + 1);
		int notTaken = solve(mask, n, pos + 1);
		
		return min(taken, notTaken);
	}
}

int main()
{
    int t, tc, x, y, z;
    int i, j, k, h;
    char ch;

#ifndef ONLINE_JUDGE
    freopen("B-small-attempt0.in", "r", stdin); 
    freopen("output.txt", "w", stdout);
#endif
   
	cin >> tc;
	for (t = 1; t <= tc; t++) {
		cin >> R >> C >> N;
		int res = solve(0, N, 0);
		
		printf("Case #%d: %d\n", t, res);
	}
    
    return 0;
}