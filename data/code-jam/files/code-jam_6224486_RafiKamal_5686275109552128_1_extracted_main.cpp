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

int arr[SIZE];

int main()
{
    int t, tc, x, y, z;
    int i, j, k, h;
    char ch;

#ifndef ONLINE_JUDGE
    freopen("B-large.in", "r", stdin); 
    freopen("output.txt", "w", stdout);
#endif
    
	cin >> tc;
    
	int N;
	for (t = 1; t <= tc; t++) {
		cin >> N;
		for (i = 0; i < N; i++) {
			cin >> arr[i];
		}
		
		int mx = *max_element(arr, arr + N);
		
		int best = mx;
		for (i = 1; i <= mx; i++) {
			int cost = i;
			for (j = 0; j < N; j++) {
				if (arr[j] > i)
					cost += (int) ceil(arr[j] * 1.0 / i) - 1;
			}
			
			best = min(best, cost);
		}
		
		printf("Case #%d: %d\n", t, best);
	}
    
    return 0;
}