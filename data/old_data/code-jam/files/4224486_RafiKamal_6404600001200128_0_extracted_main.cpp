#include <bits/stdc++.h>

#define INF 2147483647
#define LLD int
#define clr(a) memset(a,0,sizeof(a))
#define reset(a) memset(a,-1,sizeof(a))

#define PRINT(X) (cout << #X << " -> " << X << endl)
#define PI acos(0)
#define MAX_INT 2147483647
#define SIZE 2001
#define MOD 1000000007

#define BD(i, j) (i >= 0 && i < n && j >= 0 && j < m)

using namespace std;

int A[SIZE];

int main()
{
    int t, tc, x, y, z;
    int i, j, k, h;
    char ch;

#ifndef ONLINE_JUDGE
    freopen("A-small-attempt0.in", "r", stdin); 
    freopen("output.txt", "w", stdout);
#endif
    
	cin >> tc;
    
	int N;
	for (t = 1; t <= tc; t++) {
		cin >> N;
		
		int mx = 0;
		for (i = 0; i < N; i++) {
			cin >> A[i];
			if (i && A[i - 1] - A[i] > mx)
				mx = A[i - 1] - A[i];
				
		}
		
		int res1 = 0, res2 = 0;
		for (i = 1; i < N; i++) {
			if (A[i] < A[i - 1])
				res1 += A[i - 1] - A[i];
			
			
			res2 += min(A[i - 1], mx);
		}
		
		
		printf("Case #%d: %d %d\n", t, res1, res2);
	}
    
    return 0;
}