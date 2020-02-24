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

int reverse(int number) {
	int rev_number = 0;
	
	while(number)
    {
        rev_number = (rev_number * 10) + (number % 10);
        number /= 10;
    }
	
	return rev_number;
}

int main()
{
    int t, tc, x, y, z;
    int i, j, k, h;
    char ch;

#ifndef ONLINE_JUDGE
    freopen("A-small-attempt0.in", "r", stdin); 
    freopen("output.txt", "w", stdout);
#endif
    
	for (i = 1; i <= 1000000; i++)
		M[i] = MAX_INT;
	
	M[1] = 1;
	for (i = 1; i <= 1000000; i++) {
		M[i + 1] = min(M[i + 1], 1 + M[i]);
		int rev = reverse(i);
		M[rev] = min(M[rev], 1 + M[i]);
	}
    
	int N, B;
	cin >> tc;
	for (t = 1; t <= tc; t++) {
		cin >> N;
		
		printf("Case #%d: %d\n", t, M[N]);
	}
    
    return 0;
}